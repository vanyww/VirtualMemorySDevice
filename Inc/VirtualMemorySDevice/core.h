#pragma once

#include "SDeviceCore/interface.h"

#define __VIRTUAL_MEMORY_BASE_TYPE_MAX_VALUE UINT16_MAX
#define __VIRTUAL_MEMORY_MOCK_VALUE 0U

typedef uint16_t VirtualMemoryBaseType;

typedef enum
{
   VIRTUAL_MEMORY_STATUS_OK,
   VIRTUAL_MEMORY_STATUS_DATA_ERROR,
   VIRTUAL_MEMORY_STATUS_DEVICE_ERROR,
   VIRTUAL_MEMORY_STATUS_ADDRESS_ERROR
} VirtualMemoryStatus;

typedef enum
{
   VIRTUAL_MEMORY_CHUNK_STATUS_OK           = VIRTUAL_MEMORY_STATUS_OK,
   VIRTUAL_MEMORY_CHUNK_STATUS_DATA_ERROR   = VIRTUAL_MEMORY_STATUS_DATA_ERROR,
   VIRTUAL_MEMORY_CHUNK_STATUS_DEVICE_ERROR = VIRTUAL_MEMORY_STATUS_DEVICE_ERROR
} VirtualMemoryChunkStatus;

typedef struct
{
   void *Data;
   VirtualMemoryBaseType Size;
   VirtualMemoryBaseType Offset;
} VirtualMemoryChunkReadParameters;

typedef struct
{
   const void *Data;
   VirtualMemoryBaseType Size;
   VirtualMemoryBaseType Offset;
} VirtualMemoryChunkWriteParameters;

typedef struct
{
   VirtualMemoryBaseType Address;
   VirtualMemoryBaseType Size;
} VirtualMemoryParameters;

typedef struct VirtualMemoryChunk VirtualMemoryChunk;

typedef struct
{
   const VirtualMemoryChunk *Chunks;
   VirtualMemoryBaseType Count;
} VirtualMemoryChunkList;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);

typedef struct
{
   const VirtualMemoryChunkList *ChunksList;
   VirtualMemoryBaseType AddressingStart;
} __SDEVICE_CONSTANT_DATA(VirtualMemory);

typedef struct { } __SDEVICE_SETTINGS_DATA(VirtualMemory);

typedef struct
{
   VirtualMemoryBaseType AddressingEnd;
} __SDEVICE_DYNAMIC_DATA(VirtualMemory);

__SDEVICE_HANDLE_DEFINITION(VirtualMemory);

__SDEVICE_INITIALIZE_HANDLE_DECLARATION(VirtualMemory,);

typedef enum
{
   VIRTUAL_MEMORY_RUNTIME_ERROR_WRONG_ADDRESS = 0x01,
   VIRTUAL_MEMORY_RUNTIME_ERROR_CHUNK_ACCESS  = 0x02
} VirtualMemoryRuntimeError;

/* Satty's interface end */

struct VirtualMemoryChunk
{
   VirtualMemoryChunkStatus (* Read)(__SDEVICE_HANDLE(VirtualMemory) *,
                                     const VirtualMemoryChunkReadParameters *,
                                     const void *,
                                     const void *);
   VirtualMemoryChunkStatus (* Write)(__SDEVICE_HANDLE(VirtualMemory) *,
                                      const VirtualMemoryChunkWriteParameters *,
                                      const void *,
                                      const void *);
   const void *Context;
   VirtualMemoryBaseType BytesCount;
};

VirtualMemoryStatus VirtualMemoryRead(__SDEVICE_HANDLE(VirtualMemory) *,
                                      const VirtualMemoryParameters *,
                                      void *,
                                      const void *);

VirtualMemoryStatus VirtualMemoryWrite(__SDEVICE_HANDLE(VirtualMemory) *,
                                       const VirtualMemoryParameters *,
                                       const void *,
                                       const void *);
