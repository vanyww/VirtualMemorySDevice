#include "../Inc/VirtualMemorySDevice/core.h"
#include "Operations/operations.h"
#include "ChunkFinder/chunk_finder.h"

typedef VirtualMemorySDeviceStatus (*VirtualMemoryOperation)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                             void *,
                                                             const VirtualMemorySDeviceChunk *,
                                                             VirtualMemorySDeviceFunctionParameters *);

static VirtualMemorySDeviceStatus VirtualMemoryTryPerformOperation(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                                   VirtualMemoryOperation operation,
                                                                   const void *context,
                                                                   void *data,
                                                                   VirtualMemorySDeviceBaseType address,
                                                                   VirtualMemorySDeviceBaseType length)
{
   VirtualMemoryPointer memory;
   if(VirtualMemoryTryFindChunk(handle, address, &memory) != true)
      return VIRTUAL_MEMORY_SDEVICE_STATUS_DATA_ERROR;

   VirtualMemorySDeviceFunctionParameters operationParameters =
   {
      .Offset = memory.Offset,
      .CallContext = context,
      .BytesCount = __MIN(memory.Chunk->BytesCount - memory.Offset, length)
   };

   while(length > 0)
   {
      VirtualMemorySDeviceStatus operationStatus = operation(handle, data, memory.Chunk, &operationParameters);

      if(operationStatus != VIRTUAL_MEMORY_SDEVICE_STATUS_OK)
         return operationStatus;

      memory.Chunk++;
      length -= operationParameters.BytesCount;
      data += operationParameters.BytesCount;
      operationParameters.BytesCount = __MIN(memory.Chunk->BytesCount, length);

      if(operationParameters.Offset != 0)
         operationParameters.Offset = 0;
   }

   return VIRTUAL_MEMORY_SDEVICE_STATUS_OK;
}

VirtualMemorySDeviceStatus VirtualMemorySDeviceRead(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                    const void *context,
                                                    void *data,
                                                    VirtualMemorySDeviceBaseType address,
                                                    VirtualMemorySDeviceBaseType length)
{
   return VirtualMemoryTryPerformOperation(handle,
                                           VirtualMemoryTryReadChunk,
                                           context,
                                           data,
                                           address,
                                           length);
}

VirtualMemorySDeviceStatus VirtualMemorySDeviceWrite(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                     const void *context,
                                                     const void *data,
                                                     VirtualMemorySDeviceBaseType address,
                                                     VirtualMemorySDeviceBaseType length)
{
   return VirtualMemoryTryPerformOperation(handle,
                                           VirtualMemoryTryWriteChunk,
                                           context,
                                           (void *)data,
                                           address,
                                           length);
}
