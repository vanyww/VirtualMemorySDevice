#pragma once

#include "SDeviceCore/core.h"

#include <stdbool.h>

typedef struct VirtualMemorySDeviceChunk VirtualMemorySDeviceChunk;

SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(VirtualMemory);

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_OPERATION_FAIL,
   VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_CHUNK_PARAMETERS,
   VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_CHUNK_ADDRESS
} VirtualMemorySDeviceStatus;

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t ChunksCount;
};

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, parent, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer);

typedef struct
{
   void *Data;
   size_t Size;
   intptr_t Offset;
   const void *ChunkContext;
   const void *CallContext;
} VirtualMemorySDeviceChunkReadParameters;

typedef struct
{
   const void *Data;
   size_t Size;
   intptr_t Offset;
   const void *ChunkContext;
   const void *CallContext;
} VirtualMemorySDeviceChunkWriteParameters;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK               = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OPERATION_FAIL   = VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_OPERATION_FAIL,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_WRONG_PARAMETERS = VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_CHUNK_PARAMETERS
} VirtualMemorySDeviceChunkStatus;

struct VirtualMemorySDeviceChunk
{
   VirtualMemorySDeviceChunkStatus (* Read)(SDEVICE_HANDLE(VirtualMemory) *handle,
                                            const VirtualMemorySDeviceChunkReadParameters *parameters);
   VirtualMemorySDeviceChunkStatus (* Write)(SDEVICE_HANDLE(VirtualMemory) *handle,
                                             const VirtualMemorySDeviceChunkWriteParameters *parameters);
   const void *Context;
   size_t Size;
};

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
