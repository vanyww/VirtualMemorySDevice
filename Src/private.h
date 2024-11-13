#pragma once

#include "VirtualMemorySDevice/public.h"

#define IS_VALID_THIS_HANDLE(handle) SDEVICE_IS_VALID_HANDLE(VirtualMemory, handle)

#define LogOperationStatus(handle, status, address)                                                                    \
   {                                                                                                                   \
      VirtualMemorySDeviceOperationStatusLogExtras _mExtras =                                                          \
      {                                                                                                                \
         .Address = (address)                                                                                          \
      };                                                                                                               \
                                                                                                                       \
      SDeviceLogStatusWithExtras(handle, status, &_mExtras, sizeof(_mExtras));                                         \
   }

#define LogChunkGetFailStatus(handle, address)                                                                         \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_GET_FAIL, address)

#define LogChunkSetFailStatus(handle, address)                                                                         \
   LogOperationStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_SET_FAIL, address)

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   #define GET_HIGHEST_ADDRESS(handle) (                                                                               \
      {                                                                                                                \
         __auto_type _mHandle = (handle);                                                                              \
         size_t _mLastIdx = _mHandle->Init->ChunksCount - 1;                                                           \
                                                                                                                       \
         _mHandle->Runtime->AddressMap[_mLastIdx] + (_mHandle->Init->Chunks[_mLastIdx].Size - 1);                      \
      })
#else
   #define GET_HIGHEST_ADDRESS(handle) ((handle)->Runtime->HighestAddress)
#endif

typedef VirtualMemorySDeviceChunk ThisChunk;

typedef VirtualMemorySDeviceSizeType ThisSizeType;
typedef VirtualMemorySDeviceAddressType ThisAddressType;

typedef VirtualMemorySDeviceOperationType ThisOperationType;
typedef VirtualMemorySDeviceOperationParameters ThisOperationParameters;

typedef VirtualMemorySDeviceChunkGetParameters ThisChunkGetParameters;
typedef VirtualMemorySDeviceChunkSetParameters ThisChunkSetParameters;

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
