#pragma once

#include "config.h"
#include "dependencies.h"
#include "log.h"

/* 074C9AFA-6248-11EE-8C95-0492264B1031 */
#define VIRTUAL_MEMORY_SDEVICE_UUID_HIGH 0x074C9AFA624811EE
#define VIRTUAL_MEMORY_SDEVICE_UUID_LOW  0x8C950492264B1031

#define VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR 2
#define VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR 0
#define VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH 0

SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(VirtualMemory);

typedef VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE VirtualMemorySDeviceSizeType;
typedef VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE VirtualMemorySDeviceAddressType;

typedef struct
{
   void                        *Data;
   VirtualMemorySDeviceSizeType Offset;
   VirtualMemorySDeviceSizeType Size;
} VirtualMemorySDeviceChunkReadParameters;

typedef struct
{
   const void                  *Data;
   VirtualMemorySDeviceSizeType Offset;
   VirtualMemorySDeviceSizeType Size;
} VirtualMemorySDeviceChunkWriteParameters;

typedef struct VirtualMemorySDeviceChunk
{
   SDevicePropertyStatus (* Read)(
         SDEVICE_HANDLE(VirtualMemory)                 *handle,
         const struct VirtualMemorySDeviceChunk        *chunk,
         const VirtualMemorySDeviceChunkReadParameters *parameters,
         const void                                    *context);

   SDevicePropertyStatus (* Write)(
         SDEVICE_HANDLE(VirtualMemory)                  *handle,
         const struct VirtualMemorySDeviceChunk         *chunk,
         const VirtualMemorySDeviceChunkWriteParameters *parameters,
         const void                                     *context);

   const void                  *Context;
   VirtualMemorySDeviceSizeType Size;
} VirtualMemorySDeviceChunk;

typedef struct
{
   const void                     *Data;
   VirtualMemorySDeviceAddressType Address;
   VirtualMemorySDeviceSizeType    Size;
} VirtualMemorySDeviceWriteParameters;

typedef struct
{
   void                           *Data;
   VirtualMemorySDeviceAddressType Address;
   VirtualMemorySDeviceSizeType    Size;
} VirtualMemorySDeviceReadParameters;

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t                           ChunksCount;
};

SDEVICE_IDENTITY_BLOCK_DECLARATION(VirtualMemory);

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer);

SDevicePropertyStatus VirtualMemorySDeviceRead(
      SDEVICE_HANDLE(VirtualMemory)            *handle,
      const VirtualMemorySDeviceReadParameters *parameters,
      const void                               *context);

SDevicePropertyStatus VirtualMemorySDeviceWrite(
      SDEVICE_HANDLE(VirtualMemory)             *handle,
      const VirtualMemorySDeviceWriteParameters *parameters,
      const void                                *context);
