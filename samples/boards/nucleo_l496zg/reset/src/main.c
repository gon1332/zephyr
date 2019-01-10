/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <misc/reboot.h>

void main(void)
{
	printk("Reset System Demo on %s\n", CONFIG_ARCH);

    // sys_reboot(0);
}
