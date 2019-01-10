/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sample.h"
#include <gpio.h>

#define SEC_TO_US(X) ((X) * 1000000)

#define LPS1_STATE_ENTER_TO         10
#define LPS2_STATE_ENTER_TO         30
#define DEEP_SLEEP_STATE_ENTER_TO   90

struct device *gpiob;
struct device *gpioc;

void configure_gpio(void)
{
    gpioc = device_get_binding(PORT);
    gpiob = device_get_binding(LED_PORT);

    // /* Configure button 1 as deep sleep trigger event */
    // gpio_pin_configure(gpio_port, BUTTON_1, GPIO_DIR_IN
    //                                         | GPIO_PUD_PULL_UP);

    /* Configure button 2 as wake source from deep sleep */
    gpio_pin_configure(gpioc, BUTTON_2, GPIO_DIR_IN
                                        | GPIO_PUD_PULL_UP
                                        | GPIO_INT | GPIO_INT_EDGE
                                        | GPIO_INT_ACTIVE_HIGH);

    gpio_pin_enable_callback(gpioc, BUTTON_2);

    /* Configure LEDs */
    gpio_pin_configure(gpiob, LED_1, GPIO_DIR_OUT);
    gpio_pin_write(gpiob, LED_1, LED_ON);

    gpio_pin_configure(gpiob, LED_2, GPIO_DIR_OUT);
    gpio_pin_write(gpiob, LED_2, LED_ON);
}

void main(void)
{
    uint32_t level = 0U;

	printk("Power Management Demo on %s\n", CONFIG_ARCH);

            /* Enable Power Peripheral */
            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

            if (LL_PWR_IsActiveFlag_SB() != 0) {
                printk("SoC resumed from STANDBY.\n");
                LL_PWR_ClearFlag_SB();
            }

            if (LL_PWR_IsActiveFlag_WU2() != 0) {
                printk("WU2 was active.\n");
                LL_PWR_ClearFlag_WU2();
            }

    configure_gpio();

    printk("\n<-- App doing busy wait for 5 Sec -->\n");
    k_busy_wait(SEC_TO_US(5));

    /*
     * Start the demo.
     */
    // for (int i = 1; i <= 4; i++) {
    //     printk("\n<-- App doing busy wait for 10 Sec -->\n");
    //     k_busy_wait(SEC_TO_US(10));

    //     /* Create Idleness to make Idle thread run */
    //     if ((i % 2) == 0) {
    //         printk("\n<-- App going to sleep for %u Sec -->\n",
    //                LPS2_STATE_ENTER_TO);
    //         k_sleep(K_SECONDS(LPS2_STATE_ENTER_TO));
    //     } else {
    //         printk("\n<-- App going to sleep for %u Sec -->\n",
    //                LPS1_STATE_ENTER_TO);
    //         k_sleep(K_SECONDS(LPS1_STATE_ENTER_TO));
    //     }
    // }

    printk("\nPress BUTTON1 to enter into Deep Sleep state. "
            "Press BUTTON2 to exit Deep Sleep state\n");
    k_sleep(K_SECONDS(DEEP_SLEEP_STATE_ENTER_TO));

    while (1) {
        continue;
    }
}
