/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-20     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "dht.h"

#define DHT11_DATA_PIN           GET_PIN(E, 9)
#define DHT22_DATA_PIN           GET_PIN(E, 13)

/* cat_dht11 */
static void cat_dht11(void)
{
    struct dht_device dht11;
    dht_init(&dht11, SENSOR_DHT11, DHT11_DATA_PIN);

    if(dht_read(&dht11)) {

        float t = dht_get_temperature(&dht11);
        float h = dht_get_humidity(&dht11);

        rt_kprintf("(DHT11) temperature: %d.%02d'C, humidity: %d.%02d%%\n", 
                    (int)t, (int)(t*100) % 100, (int)h, (int)(h*100) % 100);
    }
    else {
        rt_kprintf("(DHT11) error\n");
    }
}

/* cat_dht22 */
static void cat_dht22(void)
{
    struct dht_device dht22;
    dht_init(&dht22, SENSOR_DHT22, DHT22_DATA_PIN);

    if(dht_read(&dht22)) {

        float t = dht_get_temperature(&dht22);
        float h = dht_get_humidity(&dht22);

        rt_kprintf("(DHT22) temperature: %d.%02d'C, humidity: %d.%02d%\n", 
                    (int)t, (int)(t*100) % 100, (int)h, (int)(h*100) % 100);
    }
    else {
        rt_kprintf("(DHT22) error\n");
    }
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_dht11, read dht11 humidity and temperature);
MSH_CMD_EXPORT(cat_dht22, read dht22 humidity and temperature);
#endif