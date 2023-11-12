#pragma once

#include "config.h"

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL
} VirtualMemorySDeviceStatus;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL
} VirtualMemorySDeviceChunkStatus;

#define VIRTUAL_MEMORY_SDEVICE_IS_VALID_CHUNK_STATUS(value) (                                                          \
{                                                                                                                      \
   __auto_type _value = (value);                                                                                       \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK              ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL;                                                      \
})

typedef struct
{
   VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE Address;
   VirtualMemorySDeviceChunkStatus     ChunkStatus;
} VirtualMemorySDeviceOperationStatusLogExtras;
