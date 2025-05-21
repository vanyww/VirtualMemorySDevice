#pragma once

#include "config.h"
#include "dependencies.h"

#define VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR 3
#define VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR 0
#define VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH 0

SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(VirtualMemory);

typedef struct
{
   void  *Data;
   size_t Offset;
   size_t Size;
} VirtualMemorySDeviceChunkGetParameters;

typedef struct
{
   const void *Data;
   size_t      Offset;
   size_t      Size;
} VirtualMemorySDeviceChunkSetParameters;

typedef struct VirtualMemorySDeviceChunk
{
   SDevicePropertyStatus (* Get)(
         SDEVICE_HANDLE(VirtualMemory)                *this,
         const struct VirtualMemorySDeviceChunk       *chunk,
         const VirtualMemorySDeviceChunkGetParameters *chunkParameters,
         const void                                   *callParameters);

   SDevicePropertyStatus (* Set)(
         SDEVICE_HANDLE(VirtualMemory)                *this,
         const struct VirtualMemorySDeviceChunk       *chunk,
         const VirtualMemorySDeviceChunkSetParameters *chunkParameters,
         const void                                   *callParameters);

   const void *Context;
   size_t      Size;
} VirtualMemorySDeviceChunk;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_WRITE,

   VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDXS_COUNT
} VirtualMemorySDeviceOperationTypeIdx;

typedef struct
{
   union
   {
      struct
      {
         void     *Data;
         uintptr_t Address;
         size_t    Size;
      } AsRead;

      struct
      {
         const void *Data;
         uintptr_t   Address;
         size_t      Size;
      } AsWrite;

      struct
      {
         const void *Data;
         uintptr_t   Address;
         size_t      Size;
      } AsAny;
   } Interface;

   VirtualMemorySDeviceOperationTypeIdx TypeIdx;
} VirtualMemorySDeviceOperationParameters;

SDEVICE_INIT_DATA_DECLARATION(VirtualMemory)
{
   const VirtualMemorySDeviceChunk *Chunks;
   size_t                           ChunksCount;
};

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, this);

SDevicePropertyStatus VirtualMemorySDeviceAccess(
      SDEVICE_HANDLE(VirtualMemory)                 *this,
      const VirtualMemorySDeviceOperationParameters *operationParameters,
      const void                                    *callParameters);
