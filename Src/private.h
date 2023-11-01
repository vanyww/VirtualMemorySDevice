#pragma once

#include "VirtualMemorySDevice/public.h"

#include "SDeviceCore/errors.h"

#define IS_VALID_THIS_HANDLE(handle) (                                                                                 \
{                                                                                                                      \
   __auto_type _handle = (handle);                                                                                     \
   _handle != NULL          &&                                                                                         \
   _handle->Init != NULL    &&                                                                                         \
   _handle->Runtime != NULL &&                                                                                         \
   SDeviceCompareIdentityBlocks(SDeviceGetHandleIdentityBlock(_handle), &SDEVICE_IDENTITY_BLOCK(VirtualMemory));       \
})

#define LogOperationStatus(handle, status, address, chunkStatus) (                                                     \
{                                                                                                                      \
   VirtualMemorySDeviceOperationStatusLogExtras _extras =                                                              \
   {                                                                                                                   \
      .Address     = (address),                                                                                        \
      .ChunkStatus = (chunkStatus)                                                                                     \
   };                                                                                                                  \
   SDeviceLogStatusWithExtras(handle, status, &_extras, sizeof(_extras));                                              \
})

#define LogReadFailStatus(handle, address, chunkStatus)                                                                \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL, address, chunkStatus)

#define LogWriteFailStatus(handle, address, chunkStatus)                                                               \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL, address, chunkStatus)

typedef VIRTUAL_MEMORY_SDEVICE_SIZE_TYPE SizeType;
typedef VIRTUAL_MEMORY_SDEVICE_ADDRESS_TYPE AddressType;

typedef VirtualMemorySDeviceChunk ChunkInternal;
typedef VirtualMemorySDeviceChunkStatus ChunkStatusInternal;
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
