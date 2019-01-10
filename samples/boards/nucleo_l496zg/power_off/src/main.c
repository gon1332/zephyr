/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <stm32l4xx_ll_pwr.h>
#include <stm32l4xx_ll_cortex.h>

#define PORT SW0_GPIO_CONTROLLER
#define PIN  SW0_GPIO_PIN
#define EDGE (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#define PULL_UP 0

#define SEC_TO_US(X) ((X) * 1000000)

static struct device *gpioc;

void configure_system_clock(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

    /* Enable MSI*/
    if (LL_RCC_MSI_IsReady() != 1) {
        LL_RCC_MSI_Enable();

        while (LL_RCC_MSI_IsReady() != 1) {
            continue;
        }
    }

    /* Configure and enable PLL */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI,
                                LL_RCC_PLLM_DIV_1,
                                40,
                                LL_RCC_PLLR_DIV_2);
    if (LL_RCC_PLL_IsReady() != 1) {
        LL_RCC_PLL_Enable();
        LL_RCC_PLL_EnableDomain_SYS();
        while (LL_RCC_PLL_IsReady() != 1) {
            continue;
        }
    }

    /* Select PLL as system clock */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
        continue;
    }

    /* Disable other clocks */
    LL_RCC_HSI_Disable();
    LL_RCC_HSE_Disable();

    /* Set APB1 & APB2 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_SetSystemCoreClock(80000000);
}

void configure_pwr(void)
{
    /* Enable Power Peripheral */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    if (LL_PWR_IsActiveFlag_SB() != 0) {
        printk("SoC resumed from STANDBY.\n");
        LL_PWR_ClearFlag_SB();
    }

    if (LL_PWR_IsActiveFlag_WU2() != 0) {
        LL_PWR_ClearFlag_WU2();
    }
}

void button_pressed(struct device *gpioc, struct gpio_callback *cb,
                    uint32_t pins)

{
    printk("Button pressed\n");
}

static struct gpio_callback gpiocb;

void configure_gpio(void)
{
    gpioc = device_get_binding(PORT);
    if (!gpioc) {
        printk("Error\n");
        return;
    }

    gpio_pin_configure(gpioc, PIN, GPIO_DIR_IN
                                        | GPIO_INT | GPIO_INT_EDGE
                                        | GPIO_INT_ACTIVE_HIGH);
    gpio_init_callback(&gpiocb, button_pressed, BIT(PIN));
    gpio_add_callback(gpioc, &gpiocb);
    gpio_pin_enable_callback(gpioc, PIN);
}

void enter_standby_mode(void)
{
    if (LL_PWR_IsActiveFlag_InternWU()) {
        LL_PWR_DisableInternWU();
    }
    /*
     * Configure wake up sources
     */
    /* Disable all used wakeup sources: WKUP pin*/
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    /* Clear wakeup Flag */
    LL_PWR_ClearFlag_WU();

    LL_PWR_SetWakeUpPinPolarityHigh(LL_PWR_WAKEUP_PIN2);

    /* Enable wakeup pin WKUP2 */
    LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    /*
     * Enter Standby mode
     */
    /* Set STANDBY mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep();

    /* Request Wait For Interrupt */
    __WFI();
}

void enter_shutdown_mode(void)
{
    if (LL_PWR_IsActiveFlag_InternWU()) {
        LL_PWR_DisableInternWU();
    }
    /*
     * Configure wake up sources
     */
    /* Disable all used wakeup sources: WKUP pin*/
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    /* Clear wakeup Flag */
    LL_PWR_ClearFlag_WU();

    LL_PWR_SetWakeUpPinPolarityHigh(LL_PWR_WAKEUP_PIN2);

    /* Enable wakeup pin WKUP2 */
    LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    /*
     * Enter Shutdown mode
     */
    /* Set SHUTDOWN mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep();

    /* Request Wait For Interrupt */
    __WFI();
}

void main(void)
{
	printk("Deep Sleep Modes Demo on %s\n", CONFIG_ARCH);

    configure_system_clock();
    configure_pwr();
    configure_gpio();

    printk("Busy wait for 5 seconds..\n");
    k_busy_wait(SEC_TO_US(5));

#if 1
    printk("Device is entering SHUTDOWN mode..\n");
    enter_shutdown_mode();
#else
    printk("Device is entering STANDBY mode..\n");
    enter_standby_mode();
#endif

    printk("After Deep Sleep. Should not see this.\n");

    while (1) {
        continue;
    }
}
