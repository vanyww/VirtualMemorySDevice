#pragma once

#include "VirtualMemorySDevice/public.h"

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
