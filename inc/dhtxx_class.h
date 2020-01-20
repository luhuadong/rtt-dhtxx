/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-20     RudyLo       the first version
 */

#ifndef __DHTXX_H__
#define __DHTXX_H__

#include <rthw.h>
#include <rtthread.h>

#define DHTLIB_VERSION       "0.0.1"

/* DHT11 timing */
#define DHT11_BEGIN_TIME     20 /* ms */
#define DHT11_PULL_TIME      30 /* us */

/* DHT22 timing */
#define DHT22_BEGIN_TIME      1 /* ms */
#define DHT22_PULL_TIME      30 /* us */

#define MEASURE_TIME         40 /* us */

enum sensor_dhtxx_type
{
	SENSOR_DHT11 = 11,
	SENSOR_DHT22 = 22
};

struct sensor_dhtxx
{
	/* property */
	rt_uint8_t  _type;
	rt_uint8_t  _pin;
	rt_uint8_t  data[5];
	rt_uint32_t begin_time;
	rt_uint32_t pull_time;

	/* function */
	int   (*read)(void);
	float (*get_humidity)(void);
	float (*get_temperature)(void);

	float (*convert_c2k)(float c);
	float (*convert_c2f)(float c);
	float (*convert_f2c)(float f);
};
typedef struct sensor_dhtxx *sensor_dhtxx_t;

sensor_dhtxx_t sensor_dhtxx_init(sensor_dhtxx_t this, sensor_dhtxx_type type, rt_uint8_t pin);


#endif /* __DHTXX_H__ */
