#pragma once

#include "VirtualMemorySDevice/public.h"

#define IS_VALID_THIS_HANDLE(handle) SDEVICE_IS_VALID_HANDLE(VirtualMemory, handle)

#define LogOperationStatus(handle, status, address)                                                                    \
   {                                                                                                                   \
      VirtualMemorySDeviceOperationStatusLogExtras $$extras =                                                          \
      {                                                                                                                \
         .Address = (address)                                                                                          \
      };                                                                                                               \
                                                                                                                       \
      SDeviceLogStatusWithExtras(handle, status, &$$extras, sizeof($$extras));                                         \
   }

#define LogReadFailStatus(handle, address)                                                                             \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL, address)

#define LogWriteFailStatus(handle, address)                                                                            \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL, address)

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   #define GET_HIGHEST_ADDRESS(handle) (                                                                               \
      {                                                                                                                \
         __auto_type $$handle = (handle);                                                                              \
         size_t      $$lastIdx = $$handle->Init->ChunksCount - 1;                                                      \
                                                                                                                       \
         $$handle->Runtime->AddressMap[$$lastIdx] + ($$handle->Init->Chunks[$$lastIdx].Size - 1);                      \
      })
#else
   #define GET_HIGHEST_ADDRESS(handle) ((handle)->Runtime->HighestAddress)
#endif

typedef VirtualMemorySDeviceChunk ThisChunk;

typedef VirtualMemorySDeviceSizeType ThisSizeType;
typedef VirtualMemorySDeviceAddressType ThisAddressType;

typedef VirtualMemorySDeviceReadParameters ThisReadParameters;
typedef VirtualMemorySDeviceWriteParameters ThisWriteParameters;

typedef VirtualMemorySDeviceChunkReadParameters ThisChunkReadParameters;
typedef VirtualMemorySDeviceChunkWriteParameters ThisChunkWriteParameters;

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(VirtualMemory);

SDEVICE_RUNTIME_DATA_DECLARATION(VirtualMemory)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   ThisAddressType *AddressMap;
#else
   ThisAddressType  HighestAddress;
#endif
};

SDEVICE_HANDLE_DECLARATION(VirtualMemory);
SDEVICE_INTERNAL_ALIASES_DECLARATION(VirtualMemory);
