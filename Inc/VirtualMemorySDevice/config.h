#pragma once

#include "SDeviceCore/config.h"
#include "SDeviceCore/common.h"

#include <stdint.h>

#if !defined(VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG)
   #define VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG false
#endif

#if VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG
   #include "virtual_memory_sdevice_config.h"
#endif

#if !defined(VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE)
   #define VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE 0
#endif

#if !defined(VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH)
   #define VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH true
#endif
