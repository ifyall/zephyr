# SPDX-License-Identifier: Apache-2.0

zephyr_include_directories(./)

zephyr_library_sources_ifdef(CONFIG_WIFI_CYW43XXX cyw43xxx_drv.c)
zephyr_compile_definitions(CYBSP_WIFI_CAPABLE)

zephyr_compile_definitions(CY_RTOS_AWARE)
