#pragma once

#include "dependencies.h"
#include "config.h"
#include "log.h"

/* 074c9afa-6248-11ee-8c95-0492264b1031 */
#define VIRTUAL_MEMORY_SDEVICE_UUID_HIGH 0x074c9afa624811ee
#define VIRTUAL_MEMORY_SDEVICE_UUID_LOW  0x8c950492264b1031

#define VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR 2
#define VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR 0
#define VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH 0

SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(VirtualMemory);

typedef struct
{
   void                            *Data;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE Offset;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE Size;
} VirtualMemorySDeviceChunkReadParameters;

typedef struct
{
   const void                      *Data;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE Offset;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE Size;
} VirtualMemorySDeviceChunkWriteParameters;

typedef struct VirtualMemorySDeviceChunk VirtualMemorySDeviceChunk;

struct VirtualMemorySDeviceChunk
{
   SDevicePropertyStatus (* Read)(SDEVICE_HANDLE(VirtualMemory)                 *handle,
                                  const VirtualMemorySDeviceChunk               *chunk,
                                  const VirtualMemorySDeviceChunkReadParameters *parameters,
                                  const void                                    *context);

   SDevicePropertyStatus (* Write)(SDEVICE_HANDLE(VirtualMemory)                  *handle,
                                   const VirtualMemorySDeviceChunk                *chunk,
                                   const VirtualMemorySDeviceChunkWriteParameters *parameters,
                                   const void                                     *context);

   const void                      *Context;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE Size;
};

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t                           ChunksCount;
};

SDEVICE_IDENTITY_BLOCK_DECLARATION(VirtualMemory);

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer);

typedef struct
{
   const void                         *Data;
   VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE Address;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE    Size;
} VirtualMemorySDeviceWriteParameters;

typedef struct
{
   void                               *Data;
   VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE Address;
   VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE    Size;
} VirtualMemorySDeviceReadParameters;

SDevicePropertyStatus VirtualMemorySDeviceRead(SDEVICE_HANDLE(VirtualMemory)            *handle,
                                               const VirtualMemorySDeviceReadParameters *parameters,
                                               const void                               *context);

SDevicePropertyStatus VirtualMemorySDeviceWrite(SDEVICE_HANDLE(VirtualMemory)             *handle,
                                                const VirtualMemorySDeviceWriteParameters *parameters,
                                                const void                                *context);
