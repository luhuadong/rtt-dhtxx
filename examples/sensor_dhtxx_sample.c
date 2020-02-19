/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-18     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "sensor_dhtxx.h"

#define DHT22_DATA_PIN    GET_PIN(E, 13)

static void cat_sensor_dht22_humi(void)
{
    rt_device_t humi_dev = RT_NULL;

    struct rt_sensor_data sensor_data;
    rt_size_t res;

    humi_dev = rt_device_find("humi_dht22");
    if (!humi_dev) {
        rt_kprintf("Can't find device humi_dht22\n");
        return;
    }

    if (rt_device_open(humi_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK) {
        rt_kprintf("Open humi_dht22 device failed!\n");
        return;
    }

    res = rt_device_read(humi_dev, 0, &sensor_data, 1);
    if (res != 1) {
        rt_kprintf("Read %d data failed!\n", res);
        rt_device_close(humi_dev);
        return;
    }

    int integer, decimal, flag;
    flag = split_int(sensor_data.data.humi, &integer, &decimal, 10);
    rt_kprintf("Humi: %s%d.%d%RH\n", flag>0?"-":"", a, b);

    rt_device_close(humi_dev);
}

static void cat_sensor_dht22_temp(void)
{
    rt_device_t temp_dev = RT_NULL;
    rt_device_t humi_dev = RT_NULL;

    struct rt_sensor_data sensor_data;
    rt_size_t res;

    temp_dev = rt_device_find("temp_dht22");
    if (!temp_dev) {
        rt_kprintf("Can't find device temp_dht22\n");
        return;
    }

    if (rt_device_open(temp_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK) {
        rt_kprintf("Open temp_dht22 device failed!\n");
        return;
    }

    res = rt_device_read(temp_dev, 0, &sensor_data, 1);
    if (res != 1) {
        rt_kprintf("Read %d data failed!\n", res);
        rt_device_close(temp_dev);
        return;
    }

    int integer, decimal, flag;
    flag = split_int(sensor_data.data.temp, &integer, &decimal, 10);
    rt_kprintf("Temp: %s%d.%d'C\n", flag>0?"-":"", a, b);

    rt_device_close(temp_dev);
}

static int rt_hw_dht22_port(void)
{
    struct rt_sensor_config cfg;
    
    cfg.intf.type = DHT22;
    cfg.intf.user_data = (void *)DHT22_DATA_PIN;
    rt_hw_dht_init("dht22", &cfg);
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_dht22_port);

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_sensor_dht22_humi, read dht22 humidity);
MSH_CMD_EXPORT(cat_sensor_dht22_temp, read dht22 temperature);
#endif