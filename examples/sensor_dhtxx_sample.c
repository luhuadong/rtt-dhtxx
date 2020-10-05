/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-18     luhuadong    the first version
 * 2020-10-05     luhuadong    v0.9.0
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "dhtxx.h"

#define DATA_PIN                 PKG_USING_DHTXX_SAMPLE_PIN

static void read_humi_entry(void *args)
{
    rt_device_t humi_dev = RT_NULL;
    struct rt_sensor_data sensor_data;

    humi_dev = rt_device_find(args);
    if (!humi_dev) 
    {
        rt_kprintf("Can't find humi device.\n");
        return;
    }

    if (rt_device_open(humi_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open humi device failed.\n");
        return;
    }

    while (1)
    {
        if (1 != rt_device_read(humi_dev, 0, &sensor_data, 1)) 
        {
            rt_kprintf("Read humi data failed.\n");
            continue;
        }
        rt_kprintf("[%d] Humi: %d\n", sensor_data.timestamp, sensor_data.data.humi);

        rt_thread_mdelay(3000);
    }

    rt_device_close(humi_dev);
}

static void read_temp_entry(void *args)
{
    rt_device_t temp_dev = RT_NULL;
    struct rt_sensor_data sensor_data;

    temp_dev = rt_device_find(args);
    if (!temp_dev) 
    {
        rt_kprintf("Can't find temp device.\n");
        return;
    }

    if (rt_device_open(temp_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open temp device failed.\n");
        return;
    }

    while(1)
    {
        if (1 != rt_device_read(temp_dev, 0, &sensor_data, 1)) 
        {
            rt_kprintf("Read temp data failed.\n");
            continue;
        }
        rt_kprintf("[%d] Temp: %d\n", sensor_data.timestamp, sensor_data.data.temp);

        rt_thread_mdelay(3000);
    }

    rt_device_close(temp_dev);
}


static int dhtxx_read_sample(void)
{
    rt_thread_t humi_thread, temp_thread;

    humi_thread = rt_thread_create("humi_th", read_humi_entry, 
                                   "humi_dht", 1024, 
                                    RT_THREAD_PRIORITY_MAX / 2, 20);
    
    
    temp_thread = rt_thread_create("temp_th", read_temp_entry, 
                                   "temp_dht", 1024, 
                                    RT_THREAD_PRIORITY_MAX / 2, 20);
    
    if (humi_thread) 
        rt_thread_startup(humi_thread);

    if (temp_thread) 
        rt_thread_startup(temp_thread);

}
INIT_APP_EXPORT(dhtxx_read_sample);


static int rt_hw_dht_port(void)
{
    struct rt_sensor_config cfg;
    
    cfg.intf.type = RT_SENSOR_INTF_ONEWIRE;
    cfg.intf.user_data = (void *)DATA_PIN;
    rt_hw_dht_init("dht", &cfg);
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_dht_port);