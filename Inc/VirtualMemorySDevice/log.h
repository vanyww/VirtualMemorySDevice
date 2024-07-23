#pragma once

#include "config.h"

#include "SDeviceCore/errors.h"

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL
} VirtualMemorySDeviceStatus;

typedef struct
{
   VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE Address;
} VirtualMemorySDeviceOperationStatusLogExtras;
