/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2016 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <soc_power.h>
#include <power.h>
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_cortex.h>
#include <stm32l4xx_ll_pwr.h>
#include <stm32l4xx_ll_utils.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(soc, CONFIG_SOC_LOG_LEVEL);

/* Invoke Low Power/System Off specific Tasks */
void sys_set_power_state(enum power_states state)
{
	switch (state) {
#ifdef CONFIG_SYS_POWER_DEEP_SLEEP
#ifdef CONFIG_SYS_POWER_STATE_DEEP_SLEEP_SUPPORTED
		case SYS_POWER_STATE_DEEP_SLEEP:
            /* Enable Backup Access for RTC registers */
            // LL_PWR_EnableBkUpAccess();

            /*
             * Configure wake up sources
             */
            /* Disable all used wakeup sources: WKUP pin*/
            LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);

            /* Clear wakeup Flag */
            LL_PWR_ClearFlag_WU2();

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

			break;
#endif
#endif
		default:
			LOG_ERR("Unsupported power state %u", state);
			break;
	}
}

/* Invoke Low Power/System Off specific Tasks */
void sys_power_state_post_ops(enum power_states state)
{
	switch (state) {
#ifdef CONFIG_SYS_POWER_DEEP_SLEEP
#ifdef CONFIG_SYS_POWER_STATE_DEEP_SLEEP_SUPPORTED
	case SYS_POWER_STATE_DEEP_SLEEP:
		/* Nothing to do. */
		break;
#endif
#endif
	default:
		LOG_ERR("Unsupported power state %u", state);
		break;
	}

	__set_BASEPRI(0);
}

bool sys_is_valid_power_state(enum power_states state)
{
	switch (state) {
#ifdef CONFIG_SYS_POWER_LOW_POWER_STATE
#ifdef CONFIG_SYS_POWER_STATE_CPU_LPS_SUPPORTED
	case SYS_POWER_STATE_CPU_LPS:
		return true;
#endif
#ifdef CONFIG_SYS_POWER_STATE_CPU_LPS_1_SUPPORTED
	case SYS_POWER_STATE_CPU_LPS_1:
		return true;
#endif
#ifdef CONFIG_SYS_POWER_STATE_CPU_LPS_2_SUPPORTED
	case SYS_POWER_STATE_CPU_LPS_2:
		return true;
#endif
#endif /* CONFIG_SYS_POWER_LOW_POWER_STATE */

#ifdef CONFIG_SYS_POWER_DEEP_SLEEP
#ifdef CONFIG_SYS_POWER_STATE_DEEP_SLEEP_SUPPORTED
	case SYS_POWER_STATE_DEEP_SLEEP:
		return true;
#endif
#ifdef CONFIG_SYS_POWER_STATE_DEEP_SLEEP_1_SUPPORTED
	case SYS_POWER_STATE_DEEP_SLEEP_1:
		return true;
#endif
#ifdef CONFIG_SYS_POWER_STATE_DEEP_SLEEP_2_SUPPORTED
	case SYS_POWER_STATE_DEEP_SLEEP_2:
		return true;
#endif
#endif /* CONFIG_SYS_POWER_DEEP_SLEEP */

	default:
		return false;
	}

	/* Not reached */
}
