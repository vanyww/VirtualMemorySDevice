#pragma once

#include "VirtualMemorySDevice/public.h"

#include "SDeviceCore/errors.h"

#define IS_VALID_THIS_HANDLE(handle) (                                                                                 \
   {                                                                                                                   \
      ThisHandle *_handle = (handle);                                                                                  \
      _handle != NULL &&                                                                                               \
      SDeviceCompareIdentityBlocks(SDeviceGetHandleIdentityBlock(_handle),                                             \
                                   &SDEVICE_IDENTITY_BLOCK(VirtualMemory));                                            \
   })

#define LogOperationStatus(handle, status, address) (                                                                  \
   {                                                                                                                   \
      VirtualMemorySDeviceOperationStatusLogExtras _extras =                                                           \
      {                                                                                                                \
         .Address = (address)                                                                                          \
      };                                                                                                               \
                                                                                                                       \
      SDeviceLogStatusWithExtras(handle, status, &_extras, sizeof(_extras));                                           \
   })

#define LogReadFailStatus(handle, address)                                                                             \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL, address)

#define LogWriteFailStatus(handle, address)                                                                            \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL, address)

typedef VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE SizeType;
typedef VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE AddressType;

typedef VirtualMemorySDeviceChunk ChunkInternal;
typedef VirtualMemorySDeviceReadParameters ReadParametersInternal;
typedef VirtualMemorySDeviceWriteParameters WriteParametersInternal;
typedef VirtualMemorySDeviceChunkReadParameters ChunkReadParametersInternal;
typedef VirtualMemorySDeviceChunkWriteParameters ChunkWriteParametersInternal;

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(VirtualMemory);

SDEVICE_RUNTIME_DATA_DECLARATION(VirtualMemory)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   AddressType *ChunkAddressMap;
#endif
   AddressType  HighestAddress;
};

SDEVICE_HANDLE_DECLARATION(VirtualMemory);
SDEVICE_INTERNAL_ALIASES_DECLARATION(VirtualMemory);
