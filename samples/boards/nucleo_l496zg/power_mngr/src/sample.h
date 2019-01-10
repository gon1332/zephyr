#include <zephyr.h>
#include <string.h>
#include <soc.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>

/* change this to use another GPIO port */
#define PORT        SW0_GPIO_CONTROLLER
// #define BUTTON_1    SW0_GPIO_PIN
#define BUTTON_2    SW0_GPIO_PIN
#define LED_PORT    LED1_GPIO_CONTROLLER
#define LED_1       LED1_GPIO_PIN
#define LED_2       LED2_GPIO_PIN

#define LOW         0
#define HIGH        1
#define LED_ON      HIGH
#define LED_OFF     LOW

extern struct device *gpiob;
