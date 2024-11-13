#pragma once

#include "SDeviceCore/config.h"
#include "SDeviceCore/common.h"

#include <stdint.h>

#if !defined(VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG) || defined(DOXYGEN)
   #define VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG false
#endif

#if VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG
   #include "virtual_memory_sdevice_config.h"
#endif

#if !defined(VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE) || defined(DOXYGEN)
   #define VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE 0x00
#endif

#if !defined(VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH) || defined(DOXYGEN)
   #define VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH true
#endif

#if !defined(VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE) || defined(DOXYGEN)
   #define VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE uintptr_t
#endif

static_assert(HAS_VALUE_UNSIGNED_TYPE(VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE));

#if !defined(VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE) || defined(DOXYGEN)
   #define VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE size_t
#endif

static_assert(HAS_VALUE_UNSIGNED_TYPE(VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE));
