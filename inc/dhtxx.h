/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-20     luhuadong    the first version
 */

#ifndef __DHTXX_H__
#define __DHTXX_H__

#include <rtthread.h>
#include <rtdevice.h>

#define DHTLIB_VERSION                       "0.9.0"
#define DHT_DATA_SIZE                        5

/* sensor model type */
#define DHT11                                0
#define DHT12                                1
#define DHT21                                2
#define DHT22                                3
#define AM2301                               DHT21
#define AM2302                               DHT22
#define AM2320                               DHT22

#if defined(PKG_USING_DHTXX_TYPE_DHT11)
#define DHT_TYPE                             DHT11
#elif defined(PKG_USING_DHTXX_TYPE_DHT12)
#define DHT_TYPE                             DHT12
#elif defined(PKG_USING_DHTXX_TYPE_DHT21)
#define DHT_TYPE                             DHT21
#elif defined(PKG_USING_DHTXX_TYPE_DHT22)
#define DHT_TYPE                             DHT22
#endif

#if defined(RT_VERSION_CHECK)
    #if (RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 0, 2))
        #define RT_SIZE_TYPE   rt_ssize_t
    #else
        #define RT_SIZE_TYPE   rt_size_t
    #endif

    #if (RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 0, 0))
        #define WEAK_DEF rt_weak
    #else
        #define WEAK_DEF RT_WEAK
#endif
#endif

struct dht_device
{
    rt_base_t   pin;
    rt_uint8_t  type;
    rt_uint8_t  data[DHT_DATA_SIZE];
    rt_mutex_t  lock;
};
typedef struct dht_device *dht_device_t;

dht_device_t dht_create(const rt_base_t pin);
void dht_delete(dht_device_t dev);

rt_err_t   dht_init(struct dht_device *dev, const rt_base_t pin);
rt_bool_t  dht_read(dht_device_t dev);
rt_int32_t dht_get_humidity(dht_device_t dev);
rt_int32_t dht_get_temperature(dht_device_t dev);

float convert_c2k(float c);
float convert_c2f(float c);
float convert_f2c(float f);

rt_int32_t split_int(const rt_int32_t num, rt_int32_t *integer, 
                     rt_int32_t *decimal, const rt_uint32_t times);

rt_err_t rt_hw_dht_init(const char *name, struct rt_sensor_config *cfg);

#endif /* __DHTXX_H__ */
