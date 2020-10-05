/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-20     luhuadong    the first version
 * 2020-10-05     luhuadong    v0.9.0
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "dhtxx.h"

#define DATA_PIN                 PKG_USING_DHTXX_SAMPLE_PIN

/* cat_dhtxx sensor data by dynamic */
static void cat_dhtxx(void)
{
    dht_device_t sensor = dht_create(DATA_PIN);

    if(dht_read(sensor)) {

        rt_int32_t temp = dht_get_temperature(sensor);
        rt_int32_t humi = dht_get_humidity(sensor);

        rt_kprintf("Temp: %d, Humi: %d\n", temp, humi);
    }
    else {
        rt_kprintf("Read dht sensor failed.\n");
    }

    dht_delete(sensor);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_dhtxx, read dhtxx humidity and temperature);
#endif