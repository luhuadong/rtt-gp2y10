/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-21     luhuadong    the first version
 */

#include "gp2y10.h"

#define DBG_TAG "sensor.sharp.gp2y10"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define ILED_PULSE_TIME          280    /* us */
#define COV_RATIO                0.17   /* (ug/m3)/mV */
#define NO_DUST_VOLTAGE          600    /* mV */
#define REFER_VOLTAGE            5000   /* mV */

#define SENSOR_DUST_RANGE_MAX    1024
#define SENSOR_DUST_RANGE_MIN    0
#define SENSOR_DUST_PERIOD_MIN   0

RT_WEAK void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;

    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
    delta = SysTick->VAL;

    while (delta - SysTick->VAL < us) continue;
}

static rt_size_t _gp2y10_polling_get_data(struct rt_sensor_device *sensor, void *buf)
{
    struct rt_sensor_data *sensor_data = buf;

    gp2y10_device_t dev = (gp2y10_device_t)sensor->config.intf.user_data;
    rt_uint32_t dust = gp2y10_get_dust_density(dev);

    sensor_data->type = RT_SENSOR_CLASS_DUST;
    sensor_data->data.dust = dust;
    sensor_data->timestamp = rt_sensor_get_ts();

    return 1;
}

static rt_size_t gp2y10_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    if (sensor->config.mode == RT_SENSOR_MODE_POLLING)
    {
        return _gp2y10_polling_get_data(sensor, buf);
    }
    else
        return 0;
}

static rt_err_t gp2y10_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        break;
    case RT_SENSOR_CTRL_SET_MODE:
        sensor->config.mode = (rt_uint32_t)args & 0xFF;
        break;
    case RT_SENSOR_CTRL_SET_RANGE:
        break;
    case RT_SENSOR_CTRL_SET_ODR:
        break;
    case RT_SENSOR_CTRL_SET_POWER:
        break;
    case RT_SENSOR_CTRL_SELF_TEST:
        break;
    default:
        break;
    }

    return result;
}

static struct rt_sensor_ops sensor_ops =
{
    gp2y10_fetch_data,
    gp2y10_control
};

/**
 * This function will init dhtxx sensor device.
 *
 * @param intf  interface 
 *
 * @return RT_EOK
 */
static rt_err_t _gp2y10_init(struct gp2y10_device *dev)
{
    RT_ASSERT(dev);

    dev->adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (dev->adc_dev == RT_NULL)
    {
        LOG_E("Can't find %s device!", ADC_DEV_NAME);
        return -RT_ERROR;
    }

    rt_pin_mode(dev->iled_pin, PIN_MODE_OUTPUT);
    rt_pin_write(dev->iled_pin, PIN_LOW);

    return RT_EOK;
}

/**
 * Call function rt_hw_gp2y10_init for initial and register a gp2y10 sensor.
 *
 * @param name  the name will be register into device framework
 * @param cfg   sensor config
 *
 * @return the result
 */
rt_err_t rt_hw_gp2y10_init(const char *name, struct rt_sensor_config *cfg)
{
    int result;
    rt_sensor_t sensor_dust = RT_NULL;

    if (_gp2y10_init((gp2y10_device_t)cfg->intf.user_data) != RT_EOK)
    {
        LOG_E("device init failed");
        return -RT_ERROR;
    }

    /* dust sensor register */
    {
        sensor_dust = rt_calloc(1, sizeof(struct rt_sensor_device));
        if (sensor_dust == RT_NULL)
        {
            LOG_E("alloc memory failed");
            return -RT_ENOMEM;
        }

        sensor_dust->info.type       = RT_SENSOR_CLASS_DUST;
        sensor_dust->info.vendor     = RT_SENSOR_VENDOR_SHARP;
        sensor_dust->info.model      = "gp2y10";
        sensor_dust->info.unit       = RT_SENSOR_UNIT_NONE;
        //sensor_dust->info.intf_type  = RT_SENSOR_INTF_ADC;
        sensor_dust->info.range_max  = SENSOR_DUST_RANGE_MAX;
        sensor_dust->info.range_min  = SENSOR_DUST_RANGE_MIN;
        sensor_dust->info.period_min = SENSOR_DUST_PERIOD_MIN;

        rt_memcpy(&sensor_dust->config, cfg, sizeof(struct rt_sensor_config));
        sensor_dust->ops = &sensor_ops;
        
        result = rt_hw_sensor_register(sensor_dust, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
        if (result != RT_EOK)
        {
            LOG_E("device register err code: %d", result);
            rt_free(sensor_dust);
            return -RT_ERROR;
        }
    }

    LOG_I("sensor init success");
    return RT_EOK;
}
