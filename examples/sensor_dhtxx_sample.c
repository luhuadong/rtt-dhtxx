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

#define DHT22_DATA_PIN           GET_PIN(E, 13)

static int rt_hw_dht22_port(void)
{
    struct rt_sensor_config cfg;
    
    cfg.intf.type = DHT22;
    cfg.intf.user_data = (void *)DHT22_DATA_PIN;
    rt_hw_dht_init("dht22", &cfg);
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_dht22_port);