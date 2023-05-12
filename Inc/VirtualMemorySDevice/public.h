#pragma once

#include "config.h"

#include <stdbool.h>

#define VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR 2
#define VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR 0
#define VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH 0
#define VIRTUAL_MEMORY_SDEVICE_CORE_VERSION ((SDeviceVersion)                                                          \
{                                                                                                                      \
   .Major = VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR,                                                                      \
   .Minor = VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR,                                                                      \
   .Patch = VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH                                                                       \
})

SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(VirtualMemory);

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS_OR_SIZE,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_PROCESSING_FAIL,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_VALIDATION_FAIL
} VirtualMemorySDeviceStatus;

#define VIRTUAL_MEMORY_SDEVICE_IS_VALID_STATUS(value) (                                                                \
{                                                                                                                      \
   __auto_type _value = (value);                                                                                       \
   _value == VIRTUAL_MEMORY_SDEVICE_STATUS_OK                    ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS         ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_PROCESSING_FAIL ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_VALIDATION_FAIL;                                                      \
})

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK              = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL = VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_PROCESSING_FAIL,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL = VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_VALIDATION_FAIL
} VirtualMemorySDeviceChunkStatus;

#define VIRTUAL_MEMORY_SDEVICE_IS_VALID_CHUNK_STATUS(value) (                                                          \
{                                                                                                                      \
   __auto_type _value = (value);                                                                                       \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK              ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL ||                                                    \
   _value == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL;                                                      \
})

typedef struct
{
   void *Data;
   intptr_t Offset;
   size_t Size;
   const void *ChunkContext;
   const void *CallContext;
} VirtualMemorySDeviceChunkReadParameters;

typedef struct
{
   const void *Data;
   intptr_t Offset;
   size_t Size;
   const void *ChunkContext;
   const void *CallContext;
} VirtualMemorySDeviceChunkWriteParameters;

typedef struct
{
   VirtualMemorySDeviceChunkStatus (* Read)(SDEVICE_HANDLE(VirtualMemory) *handle,
                                            const VirtualMemorySDeviceChunkReadParameters *parameters);
   VirtualMemorySDeviceChunkStatus (* Write)(SDEVICE_HANDLE(VirtualMemory) *handle,
                                             const VirtualMemorySDeviceChunkWriteParameters *parameters);
   const void *Context;
   size_t Size;
} VirtualMemorySDeviceChunk;

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t ChunksCount;
};

SDEVICE_STRING_NAME_DECLARATION(VirtualMemory);

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer);

typedef struct
{
   uintptr_t Address;
   size_t Size;
   const void *Data;
   const void *CallContext;
} VirtualMemorySDeviceWriteParameters;

typedef struct
{
   uintptr_t Address;
   size_t Size;
   void *Data;
   const void *CallContext;
} VirtualMemorySDeviceReadParameters;

bool VirtualMemorySDeviceTryRead(SDEVICE_HANDLE(VirtualMemory) *handle,
                                 const VirtualMemorySDeviceReadParameters *parameters);
bool VirtualMemorySDeviceTryWrite(SDEVICE_HANDLE(VirtualMemory) *handle,
                                  const VirtualMemorySDeviceWriteParameters *parameters);
