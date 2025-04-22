#pragma once

#include "config.h"
#include "dependencies.h"

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
} VirtualMemorySDeviceChunkGetParameters;

typedef struct
{
   const void                  *Data;
   VirtualMemorySDeviceSizeType Offset;
   VirtualMemorySDeviceSizeType Size;
} VirtualMemorySDeviceChunkSetParameters;

typedef struct VirtualMemorySDeviceChunk
{
   SDevicePropertyStatus (* Get)(
         SDEVICE_HANDLE(VirtualMemory)                *handle,
         const struct VirtualMemorySDeviceChunk       *chunk,
         const VirtualMemorySDeviceChunkGetParameters *parameters,
         const void                                   *context);

   SDevicePropertyStatus (* Set)(
         SDEVICE_HANDLE(VirtualMemory)                *handle,
         const struct VirtualMemorySDeviceChunk       *chunk,
         const VirtualMemorySDeviceChunkSetParameters *parameters,
         const void                                   *context);

   const void                  *Context;
   VirtualMemorySDeviceSizeType Size;
} VirtualMemorySDeviceChunk;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,

   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPES_COUNT
} VirtualMemorySDeviceOperationType;

typedef struct
{
   VirtualMemorySDeviceOperationType Type;

   union
   {
      struct
      {
         const void                     *Data;
         VirtualMemorySDeviceAddressType Address;
         VirtualMemorySDeviceSizeType    Size;
      } AsAny;

      struct
      {
         void                           *Data;
         VirtualMemorySDeviceAddressType Address;
         VirtualMemorySDeviceSizeType    Size;
      } AsRead;

      struct
      {
         const void                     *Data;
         VirtualMemorySDeviceAddressType Address;
         VirtualMemorySDeviceSizeType    Size;
      } AsWrite;
   };
} VirtualMemorySDeviceOperationParameters;

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t                           ChunksCount;
};

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer);

SDevicePropertyStatus VirtualMemorySDeviceInvokeOperation(
      SDEVICE_HANDLE(VirtualMemory)                 *handle,
      const VirtualMemorySDeviceOperationParameters *operationParameters,
      const void                                    *callParameters);
