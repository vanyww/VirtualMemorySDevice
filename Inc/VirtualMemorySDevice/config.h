#pragma once

#include "SDeviceCore/core.h"
#include "SDeviceCore/config.h"

#if (SDEVICE_CORE_VERSION_MAJOR != 8) || (SDEVICE_CORE_VERSION_MINOR < 0)
#error SDevice core version is incorrect.
#endif

#ifdef VIRTUAL_MEMORY_SDEVICE_USE_EXTERNAL_CONFIG
#include "virtual_memory_sdevice_config.h"
#endif

#ifndef VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE
#define VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE 0x00
#endif

#ifndef VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
#define VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH true
#endif
