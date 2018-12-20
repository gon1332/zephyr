#ifndef _CLOCK_H
#define _CLOCK_H

#include <inttypes.h>

typedef struct {
    uint32_t src;
    uint32_t m;
    uint32_t n;
    uint32_t r;
} pll_config_t;

/**
 * @brief Enable HSI
 */
extern void clock_enable_hsi(void);

/**
 * @brief Enable MSI
 */
extern void clock_enable_msi(void);

/**
 * @brief Enable PLL
 */
extern void clock_enable_pll(void);

/**
 * @brief Disable HSI
 */
extern void clock_disable_hsi(void);

/**
 * @brief Disable MSI
 */
extern void clock_disable_msi(void);

/**
 * @brief Disable PLL
 */
extern void clock_disable_pll(void);

/**
 * @brief Switch to HSI
 */
extern void clock_switch_to_hsi(void);

/**
 * @brief Switch to MSI
 */
extern void clock_switch_to_msi(void);

/**
 * @brief Switch to PLL
 */
extern void clock_switch_to_pll(void);

/**
 * @brief Configure PLL
 */
extern void clock_config_pll(pll_config_t *cfg);

#endif /* _CLOCK_H */
