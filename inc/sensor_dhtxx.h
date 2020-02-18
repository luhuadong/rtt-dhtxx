/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-17     RudyLo       the first version
 */

#ifndef __SENSOR_DHTXX_H__
#define __SENSOR_DHTXX_H__

#include <rtthread.h>
#include <rtdevice.h>

#define DHT11                0
#define DHT12                1
#define DHT21                2
#define DHT22                3
#define AM2301               DHT21
#define AM2302               DHT22

int rt_hw_dht_init(const char *name, struct rt_sensor_config *cfg);

#endif /* __SENSOR_DHTXX_H__ */
