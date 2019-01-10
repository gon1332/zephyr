zephyr_include_directories(${ZEPHYR_BASE}/drivers)
zephyr_sources(
  power.c
  soc.c
  )
zephyr_sources_ifdef(CONFIG_GPIO soc_gpio.c)
