cmake_minimum_required(VERSION 3.8.2)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(nucleo_l496zg_lprun_mode)

target_sources(app PRIVATE src/main.c src/power.c)
