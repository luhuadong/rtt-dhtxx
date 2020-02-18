/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-17     luhuadong    the first version
 */

#include "sensor_dhtxx.h"

#define DHTLIB_VERSION       "0.0.1"

/* timing */
#define DHT11_BEGIN_TIME     20  /* ms */
#define DHT2x_BEGIN_TIME     1   /* ms */
#define DHTxx_PULL_TIME      30  /* us */
#define DHTxx_REPLY_TIME     100 /* us */
#define MEASURE_TIME         40  /* us */

static struct dht_model_table[] = {"dht11", "dht12", "dht21", "dht22"};


RT_WEAK void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;

    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
    delta = SysTick->VAL;

    while (delta - SysTick->VAL < us) continue;
}

/**
 * This function will read a bit from sensor.
 *
 * @param pin  the pin of Dout
 *
 * @return the bit value
 */
static uint8_t dht_read_bit(const rt_base_t pin)
{
	uint8_t retry = 0;

	while(rt_pin_read(pin) && retry < DHTxx_REPLY_TIME)
    {
        retry++;
        rt_hw_us_delay(1);
    }

	retry = 0;
	while(!rt_pin_read(pin) && retry < DHTxx_REPLY_TIME)
    {
        retry++;
        rt_hw_us_delay(1);
    }

	rt_hw_us_delay(MEASURE_TIME);
	
	return rt_pin_read(pin);
}

/**
 * This function will read a byte from sensor.
 *
 * @param pin  the pin of Dout
 *
 * @return the byte
 */
static uint8_t dht_read_byte(const rt_base_t pin)
{
	uint8_t i, byte = 0;

    for(i=0; i<8; i++)
    {
        byte <<= 1;
		byte |= dht_read_bit(pin);
    }

    return byte;
}

/**
 * This function will init dhtxx sensor device.
 *
 * @param dev  the device to init
 * @param type the type of sensor
 * @param pin  the pin of Dout
 *
 * @return the device handler
 */
static int dht_init(struct rt_sensor_intf *intf)
{
    /* type <= intf->type, pin <= intf->user_data */
    /* I don't know what can I do */

    return RT_EOK;
}

/**
 * This function will read and update data array.
 *
 * @param dev  the device to be operated
 *
 * @return RT_TRUE if read successfully, otherwise return RT_FALSE.
 */
rt_bool_t dht_read(dht_device_t dev)
{
	RT_ASSERT(dev);

	uint8_t i, retry = 0, sum = 0;

	/* Reset data buffer */
	rt_memset(dev->data, 0, DHT_DATA_SIZE);

	/* MCU request sampling */
	rt_pin_mode(dev->pin, PIN_MODE_OUTPUT);
    rt_pin_write(dev->pin, PIN_LOW);
    rt_thread_mdelay(dev->begin_time);             /* Tbe */

    rt_pin_mode(dev->pin, PIN_MODE_INPUT_PULLUP);
    rt_hw_us_delay(DHTxx_PULL_TIME);               /* Tgo */

    /* Waiting for sensor reply */
    while (rt_pin_read(dev->pin) && retry < DHTxx_REPLY_TIME)
    {
        retry++;
        rt_hw_us_delay(1);                         /* Trel */
    }
    if(retry >= DHTxx_REPLY_TIME) return RT_FALSE;

    retry = 0;
    while (!rt_pin_read(dev->pin) && retry < DHTxx_REPLY_TIME)
    {
        retry++;
        rt_hw_us_delay(1);                         /* Treh */
    };
    if(retry >= DHTxx_REPLY_TIME) return RT_FALSE;

    /* Read data */
    for(i=0; i<DHT_DATA_SIZE; i++)
	{
		dev->data[i] = dht_read_byte(dev->pin);
	}

	/* Checksum */
    for(i=0; i<DHT_DATA_SIZE-1; i++)
	{
		sum += dev->data[i];
	}
	if(sum != dev->data[4]) return RT_FALSE;

	return RT_TRUE;
}

/**
 * This function will get the humidity from dhtxx sensor.
 *
 * @param dev  the device to be operated
 *
 * @return the humidity value
 */
float dht_get_humidity(dht_device_t const dev)
{
	RT_ASSERT(dev);

	float h = 0.0;

	switch(dev->type)
	{
	case SENSOR_DHT11:
		h = dev->data[0] + dev->data[1] * 0.1;
		break;
	case SENSOR_DHT21:
	case SENSOR_DHT22:
		h = ((dev->data[0] << 8) + dev->data[1]) * 0.1;
		break;
	default:
		break;
	}

	return h;
}

/**
 * This function will get the temperature from dhtxx sensor.
 *
 * @param dev  the device to be operated
 *
 * @return the temperature value
 */
float dht_get_temperature(dht_device_t const dev)
{
	RT_ASSERT(dev);

	float t = 0.0;

	switch(dev->type)
	{
	case SENSOR_DHT11:
		t = dev->data[2] + dev->data[3] * 0.1;
		break;
	case SENSOR_DHT21:
	case SENSOR_DHT22:
		t = (((dev->data[2] & 0x7f) << 8) + dev->data[3]) * 0.1;
		if(dev->data[2] & 0x80) {
			t = -t;
		}
		break;
	default:
		break;
	}

	return t;
}

static rt_size_t dht_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    RT_ASSERT(sensor);
    RT_ASSERT(buf);

    if (sensor->config.mode == RT_SENSOR_MODE_POLLING)
    {
        return _ds18b20_polling_get_data(sensor, buf);
    }
    else
        return 0;
}
static rt_size_t bme280_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    struct bme280_data comp_data;
    struct rt_sensor_data *data = buf;
        
    if (sensor->info.type == RT_SENSOR_CLASS_BARO)
    {
        bme280_get_sensor_data(BME280_PRESS, &comp_data, &_bme280_dev);

        data->type = RT_SENSOR_CLASS_BARO;
        data->data.baro = comp_data.pressure;
        data->timestamp = rt_sensor_get_ts();
    }
    else if (sensor->info.type == RT_SENSOR_CLASS_TEMP)
    {
        bme280_get_sensor_data(BME280_TEMP, &comp_data, &_bme280_dev);

        data->type = RT_SENSOR_CLASS_TEMP;
        data->data.temp = comp_data.temperature / 10;
        data->timestamp = rt_sensor_get_ts();
    }
    else if (sensor->info.type == RT_SENSOR_CLASS_HUMI)
    {
        bme280_get_sensor_data(BME280_HUM, &comp_data, &_bme280_dev);

        data->type = RT_SENSOR_CLASS_HUMI;
        data->data.humi = comp_data.humidity / 100;
        data->timestamp = rt_sensor_get_ts();
    }
    return 1;
}

static rt_err_t dht_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    return result;
}

static struct rt_sensor_ops sensor_ops =
{
    dht_fetch_data,
    dht_control
};

/**
 * This function will convert temperature in degree Celsius to Kelvin.
 *
 * @param c  the temperature indicated by degree Celsius
 *
 * @return the result
 */
int rt_hw_dht_init(const char *name, struct rt_sensor_config *cfg)
{
    int result;
    rt_sensor_t sensor_temp = RT_NULL, sensor_humi = RT_NULL;
    struct rt_sensor_module *dht_module = RT_NULL;

    if (dht_init(&cfg->intf) != RT_EOK)
    {
        return RT_ERROR;
    }
    
    module = rt_calloc(1, sizeof(struct rt_sensor_module));
    if (module == RT_NULL)
    {
        return -1;
    }
    module->sen[0] = sensor_humi;
    module->sen[1] = sensor_temp;
    module->sen_num = 2;

    //char *model_name = dht_model_table[(dht_info_t)(cfg->intf.user_data)->type];

    /* humidity sensor register */
    {
        sensor_humi = rt_calloc(1, sizeof(struct rt_sensor_device));
        if (sensor_humi == RT_NULL)
            goto __exit;

        sensor_humi->info.type       = RT_SENSOR_CLASS_HUMI;
        sensor_humi->info.vendor     = RT_SENSOR_VENDOR_AOSONG;
        sensor_humi->info.model      = dht_model_table[cfg->intf.type];
        sensor_humi->info.unit       = RT_SENSOR_UNIT_PERMILLAGE;
        sensor_humi->info.intf_type  = RT_SENSOR_INTF_ONEWIRE;
        sensor_baro->info.range_max  = 1000;
        sensor_baro->info.range_min  = 0;
        sensor_humi->info.period_min = 2000;

        rt_memcpy(&sensor_humi->config, cfg, sizeof(struct rt_sensor_config));
        sensor_humi->ops = &sensor_ops;
        sensor_humi->module = module;
        
        result = rt_hw_sensor_register(sensor_humi, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
        if (result != RT_EOK)
        {
            LOG_E("device register err code: %d", result);
            goto __exit;
        }
    }

    /* temperature sensor register */
    {
        sensor_temp = rt_calloc(1, sizeof(struct rt_sensor_device));
        if (sensor_temp == RT_NULL)
            goto __exit;

        sensor_temp->info.type       = RT_SENSOR_CLASS_TEMP;
        sensor_temp->info.vendor     = RT_SENSOR_VENDOR_AOSONG;
        sensor_temp->info.model      = dht_model_table[cfg->intf.type];
        sensor_temp->info.unit       = RT_SENSOR_UNIT_DCELSIUS;
        sensor_temp->info.intf_type  = RT_SENSOR_INTF_ONEWIRE;
        sensor_baro->info.range_max  = 800;
        sensor_baro->info.range_min  = -400;
        sensor_temp->info.period_min = 2000;

        rt_memcpy(&sensor_temp->config, cfg, sizeof(struct rt_sensor_config));
        sensor_temp->ops = &sensor_ops;
        sensor_temp->module = module;
        
        result = rt_hw_sensor_register(sensor_temp, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
        if (result != RT_EOK)
        {
            LOG_E("device register err code: %d", result);
            goto __exit;
        }
    }
    
    LOG_I("sensor init success");
    
    return RT_EOK;
    
__exit:
    if(sensor_humi)
        rt_free(sensor_humi);

    if(sensor_temp)
        rt_free(sensor_temp);

    if (module)
        rt_free(module);

    return -RT_ERROR;
}
