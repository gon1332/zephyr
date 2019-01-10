/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

#include <stm32l4xx_ll_pwr.h>
#include <stm32l4xx_ll_cortex.h>

#define PORT	SW0_GPIO_CONTROLLER
#define PIN     SW0_GPIO_PIN
#define EDGE    (GPIO_INT_ACTIVE_LOW | GPIO_INT_EDGE)
// #define PULL_UP GPIO_PUD_PULL_DOWN
#define PULL_UP 0

/* Sleep time */
#define SLEEP_TIME	500

#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

int button_get_state(void);

void enter_standby_mode(void)
{
    while (button_get_state() == 1) {
        continue;
    }

    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    LL_PWR_ClearFlag_WU();

    LL_PWR_SetWakeUpPinPolarityHigh(LL_PWR_WAKEUP_PIN2);

    LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);

    LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

    LL_LPM_EnableDeepSleep();

    __WFI();
}

void button_pressed(struct device *gpiob, struct gpio_callback *cb,
		    u32_t pins)
{
	printk("Button pressed at %d\n", k_cycle_get_32());

    // enter_standby_mode();
}

static struct gpio_callback gpio_cb;
static struct device *gpiob;
static struct device *gpiol;

static uint32_t led_speed = LED_BLINK_FAST;

void led_setup(void)
{
	gpiol = device_get_binding(LED0_GPIO_CONTROLLER);
	gpio_pin_configure(gpiol, LED0_GPIO_PIN, GPIO_DIR_OUT);
}

void button_setup(void)
{
	gpiob = device_get_binding(PORT);
	if (!gpiob) {
		printk("error\n");
		return;
	}

	gpio_pin_configure(gpiob, PIN,
			   GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));

	gpio_add_callback(gpiob, &gpio_cb);
	gpio_pin_enable_callback(gpiob, PIN);

}

int button_get_state(void)
{
    u32_t val = 0U;
    gpio_pin_read(gpiob, PIN, &val);
    return val;
}

void led_blink(uint32_t period)
{
	int cnt = 0;

	while (1) {
		gpio_pin_write(gpiol, LED0_GPIO_PIN, cnt % 2);
		cnt++;
		k_sleep(period);
	}
}

void power_init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    if (LL_PWR_IsActiveFlag_SB() != 0) {
        LL_PWR_ClearFlag_SB();

        led_speed = LED_BLINK_SLOW;

        while (button_get_state() == 1) {
            continue;
        }
    }

    if (LL_PWR_IsActiveFlag_WU2() != 0) {
        LL_PWR_ClearFlag_WU2();
    }
}

void main(void)
{
	printk("Press the user defined button on the board\n");

    // power_init();

    button_setup();
    led_setup();

    // led_blink(led_speed);

	while (1) {
		// u32_t val = 0U;

		// gpio_pin_read(gpiob, PIN, &val);
		// k_sleep(SLEEP_TIME);

        // printk("Button state: %d\n", button_get_state());

        // printk("Wait For Interrupt\n");
        // __WFI();
        k_cpu_idle();
	}
}
