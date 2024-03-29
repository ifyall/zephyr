# Copyright (c) 2020 Linumiz
# Copyright (c) 2022 Cypress Semiconductor Corporation.
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_HAS_XMCLIB OR CONFIG_SOC_FAMILY_PSOC6 OR CONFIG_SOC_FAMILY_INFINEON_CAT1)
  zephyr_library_named(modules_hal_infineon)
endif()

## Add PDL sources for XMC devices
if (CONFIG_HAS_XMCLIB)
  add_subdirectory(${ZEPHYR_HAL_INFINEON_MODULE_DIR}/XMCLib XMCLib)
endif()

if (CONFIG_SOC_FAMILY_INFINEON_CAT1A OR CONFIG_SOC_FAMILY_PSOC6)
  ## Add core-lib sources for CAT1 devices
  add_subdirectory(core-lib)

  ## Add mtb-pdl-cat1 sources for CAT1 devices
  add_subdirectory(mtb-pdl-cat1)
endif()


## Add mtb-hal-cat1 sources for CAT1 devices
if (CONFIG_SOC_FAMILY_INFINEON_CAT1A)
  add_subdirectory(mtb-hal-cat1)
endif()

## Add abstraction-rtos sources
if (CONFIG_SOC_FAMILY_INFINEON_CAT1)
  add_subdirectory(abstraction-rtos)
endif()

## Add whd-bsp-integration for CYW43xx WIFI devices
if (CONFIG_WIFI_CYW43XXX)
  add_subdirectory(whd-bsp-integration)
endif()
