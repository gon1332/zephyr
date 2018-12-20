#include "clock.h"
#include <zephyr.h>

void clock_enable_hsi(void)
{
    if (LL_RCC_HSI_IsReady() != 1) {
        LL_RCC_HSI_Enable();

        while (LL_RCC_HSI_IsReady() != 1) {
            continue;
        }
    }
}

void clock_enable_msi(void)
{
    if (LL_RCC_MSI_IsReady() != 1) {
        LL_RCC_MSI_Enable();

        while (LL_RCC_MSI_IsReady() != 1) {
            continue;
        }
    }
}

void clock_enable_pll(void)
{
    if (LL_RCC_PLL_IsReady() != 1) {
        LL_RCC_PLL_Enable();
        while (LL_RCC_PLL_IsReady() != 1) {
            continue;
        }
    }
}

void clock_disable_hsi(void)
{
    if (LL_RCC_HSI_IsReady() != 0) {
        LL_RCC_HSI_Disable();

        while (LL_RCC_HSI_IsReady() != 0) {
            continue;
        }
    }
}

void clock_disable_msi(void)
{
    if (LL_RCC_MSI_IsReady() != 0) {
        LL_RCC_MSI_Disable();

        while (LL_RCC_MSI_IsReady() != 0) {
            continue;
        }
    }
}

void clock_disable_pll(void)
{
    if (LL_RCC_PLL_IsReady() != 0) {
        LL_RCC_PLL_Disable();
        while (LL_RCC_PLL_IsReady() != 0) {
            continue;
        }
    }
}

void clock_switch_to_hsi(void)
{
    /* Select HSI as system clock */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {
        continue;
    }
}

void clock_switch_to_msi(void)
{
    /* Select MSI as system clock */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI) {
        continue;
    }
}

void clock_switch_to_pll(void)
{
    /* Select PLL as system clock */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
        continue;
    }
}

void clock_config_pll(pll_config_t *cfg)
{
    switch (cfg->src) {
        case LL_RCC_PLLSOURCE_HSI:
            LL_RCC_PLL_ConfigDomain_SYS(cfg->src, cfg->m, cfg->n, cfg->r);
            break;
        default:
            printk("Unknown clk source\n");
            break;
    }
}
