/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-20     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "gp2y10.h"

#define GP2Y10_ILED_PIN          PKG_USING_GP2Y10_SAMPLE_ILED_PIN
#define GP2Y10_AOUT_PIN          PKG_USING_GP2Y10_SAMPLE_AOUT_PIN

/* cat_gp2y10 by dynamic */
static void cat_gp2y10(void)
{
    gp2y10_device_t gp2y10 = gp2y10_create(GP2Y10_ILED_PIN, GP2Y10_AOUT_PIN);

    if (gp2y10) {

        rt_uint32_t dust = gp2y10_get_dust_density(gp2y10);
        rt_kprintf("Dust: %d ug/m3\n", dust);
    }
    else {
        rt_kprintf("Read gp2y10 sensor failed.\n");
    }

    gp2y10_delete(gp2y10);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_gp2y10, read gp2y10 dust density);
#endif