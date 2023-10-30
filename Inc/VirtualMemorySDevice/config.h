#pragma once

#include "SDeviceCore/config.h"

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
