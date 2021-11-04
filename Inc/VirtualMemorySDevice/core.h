#pragma once

#include "SDeviceCore/interface.h"

#define __VIRTUAL_MEMORY_SDEVICE_BASE_TYPE_MAX_VALUE UINT16_MAX
#define __VIRTUAL_MEMORY_SDEVICE_MOCK_VALUE 0U

typedef uint16_t VirtualMemorySDeviceBaseType;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_STATUS_DATA_ERROR,
   VIRTUAL_MEMORY_SDEVICE_STATUS_DEVICE_ERROR,
   VIRTUAL_MEMORY_SDEVICE_STATUS_ADDRESS_ERROR
} VirtualMemorySDeviceStatus;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_CHUNK_FUNCTION_STATUS_OK = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_FUNCTION_STATUS_DATA_ERROR = VIRTUAL_MEMORY_SDEVICE_STATUS_DATA_ERROR,
   VIRTUAL_MEMORY_SDEVICE_CHUNK_FUNCTION_STATUS_DEVICE_ERROR = VIRTUAL_MEMORY_SDEVICE_STATUS_DEVICE_ERROR
} VirtualMemorySDeviceChunkFunctionStatus;

typedef enum
{
   VIRTUAL_MEMORY_SDEVICE_RUNTIME_WRONG_ADDRESS_ERROR  = 0x01,
   VIRTUAL_MEMORY_SDEVICE_RUNTIME_CHUNK_FUNCTION_ERROR = 0x02
} VirtualMemorySDeviceRuntimeError;

typedef struct
{
   const void *CallContext;
   VirtualMemorySDeviceBaseType Offset;
   VirtualMemorySDeviceBaseType BytesCount;
} VirtualMemorySDeviceFunctionParameters;

typedef struct VirtualMemorySDeviceChunk VirtualMemorySDeviceChunk;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DECLARATION(VirtualMemory);

typedef struct
{
   void *Context;
   const VirtualMemorySDeviceChunk *Chunks;
   VirtualMemorySDeviceBaseType ChunksCount;
   VirtualMemorySDeviceBaseType AddressingStart;
} __SDEVICE_CONSTANT_DATA(VirtualMemory);

typedef struct { } __SDEVICE_SETTINGS_DATA(VirtualMemory);

typedef struct
{
   VirtualMemorySDeviceBaseType AddressingEnd;
} __SDEVICE_DYNAMIC_DATA(VirtualMemory);

__SDEVICE_HANDLE_DEFINITION(VirtualMemory);

__SDEVICE_INITIALIZE_HANDLE_DECLARATION(VirtualMemory,);

/* Satty's interface end */

struct VirtualMemorySDeviceChunk
{
   VirtualMemorySDeviceChunkFunctionStatus (* ReadFunction)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                            const VirtualMemorySDeviceFunctionParameters *,
                                                            void *,
                                                            const void *);
   VirtualMemorySDeviceChunkFunctionStatus (* WriteFunction)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                             const VirtualMemorySDeviceFunctionParameters *,
                                                             const void *,
                                                             const void *);
   const void *Context;
   VirtualMemorySDeviceBaseType BytesCount;
};

VirtualMemorySDeviceStatus VirtualMemorySDeviceRead(__SDEVICE_HANDLE(VirtualMemory) *,
                                                    const void *,
                                                    void *,
                                                    VirtualMemorySDeviceBaseType,
                                                    VirtualMemorySDeviceBaseType);

VirtualMemorySDeviceStatus VirtualMemorySDeviceWrite(__SDEVICE_HANDLE(VirtualMemory) *,
                                                     const void *,
                                                     const void *,
                                                     VirtualMemorySDeviceBaseType,
                                                     VirtualMemorySDeviceBaseType);
