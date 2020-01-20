/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-20     RudyLo       the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "dht11.h"

sensor_dhtxx_t sensor_dhtxx_init(sensor_dhtxx_t this, sensor_dhtxx_type type, rt_uint8_t pin)
{
	assert()
	
	this->_type = type;
	this->_pin  = pin;

	this->read = read;
	this->get_humidity = get_humidity;
	this->get_temperature = get_temperature;
	this->convert_c2k = convert_c2k;
	this->convert_c2f = convert_c2f;
	this->convert_f2c = convert_f2c;

	return this;
}

static int read(void)
{

}

static float get_humidity(void)
{

}

static float get_temperature(void)
{

}

static float convert_c2k(float c)
{

}

static float convert_c2f(float c)
{

}

static float convert_f2c(float f)
{

}
