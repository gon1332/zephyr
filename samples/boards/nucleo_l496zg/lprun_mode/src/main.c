/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "clock.h"

#include <zephyr.h>
#include <misc/printk.h>
#include <sys_clock.h>

#define SEC_TO_US(X) ((X) * 1000000)

typedef enum freq {
    FREQ_80MHZ,
    FREQ_40MHZ,
    FREQ_1MHZ,

    FREQ_MAX_NUM,
} freq_t;

static pll_config_t pll_hsi_cfg[FREQ_MAX_NUM] = {
    { LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_2, 40, LL_RCC_PLLR_DIV_4 },/* 80MHz */
    { LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 20, LL_RCC_PLLR_DIV_2 },/* 40MHz */
};

#if defined(CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME)
extern int z_clock_hw_cycles_per_sec;
#endif

static void frequency_set(freq_t freq)
{
    pll_config_t *cfg = &pll_hsi_cfg[freq];

    /* Switch to HSI and disable PLL.
     * - Switching to HSI ensures that we have a SYSCLK source once we turn off
     *   the PLL
     * - PLL must be disabled before changing its configuration
     */
    clock_switch_to_hsi();

    clock_disable_msi();
    clock_disable_pll();

    /* Change PLL Configuration */
    clock_config_pll(cfg);

    /* Enable PLL */
    clock_enable_pll();

    /* Set PLL as SYSCLK source */
    clock_switch_to_pll();
}

static void cycle_frequencies(void)
{
    for (int i = 0; i < 5; i++) {
        switch (i % FREQ_MAX_NUM) {
            case FREQ_80MHZ:
                printk("Change to 80MHZ\n");
                frequency_set(FREQ_80MHZ);
                z_clock_hw_cycles_per_sec = 80000000;
                break;
            case FREQ_40MHZ:
                printk("Change to 40MHZ\n");
                frequency_set(FREQ_40MHZ);
                z_clock_hw_cycles_per_sec = 40000000;
                break;
            default:
                printk("Unhandled frequency\n");
                break;
        }
        printk("Busy wait for 10 secs.\n");
        k_busy_wait(SEC_TO_US(10));
    }
}

static void system_clock_config(void)
{
    clock_enable_hsi();
}

void main(void)
{
	printk("Power Management Demo on %s\n", CONFIG_ARCH);

    system_clock_config();

    cycle_frequencies();

	printk("**Power States test complete**\n");
}
