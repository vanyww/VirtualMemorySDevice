#include "../Inc/VirtualMemorySDevice/core.h"

#include "Operations/operations.h"

#include <stddef.h>

static VirtualMemoryStatus PerformVirtualMemoryOperation(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                         VirtualMemoryChunkOperation operation,
                                                         const VirtualMemoryParameters *parameters,
                                                         const void *buffer,
                                                         const void *callContext)
{
   VirtualMemoryPointer memory;
   if(TryGetVirtualMemoryPointer(handle, parameters->Address, &memory) != true)
   {
      SDeviceRuntimeErrorRaised(handle, VIRTUAL_MEMORY_RUNTIME_ERROR_WRONG_ADDRESS);
      return VIRTUAL_MEMORY_STATUS_ADDRESS_ERROR;
   }

   VirtualMemoryBaseType leftToRead = parameters->Size;
   VirtualMemoryChunkParameters chunkParameters =
   {
      .AsCommon =
      {
        .Data = buffer,
        .Offset = memory.Offset,
        .Size = __MIN(memory.Chunk->BytesCount - memory.Offset, leftToRead)
      }
   };

   while(leftToRead > 0)
   {
      VirtualMemoryStatus status = operation(handle, memory.Chunk, &chunkParameters, callContext);
      if(status != VIRTUAL_MEMORY_STATUS_OK)
      {
         SDeviceRuntimeErrorRaised(handle, VIRTUAL_MEMORY_RUNTIME_ERROR_CHUNK_ACCESS);
         return status;
      }

      memory.Chunk++;
      leftToRead -= chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.Data += chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.Size = __MIN(memory.Chunk->BytesCount, leftToRead);

      if(chunkParameters.AsCommon.Offset != 0)
         chunkParameters.AsCommon.Offset = 0;
   }

   return VIRTUAL_MEMORY_STATUS_OK;
}

VirtualMemoryStatus VirtualMemoryRead(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                      const VirtualMemoryParameters *parameters,
                                      void *buffer,
                                      const void *callContext)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(handle->IsInitialized == true);
   SDeviceAssert(buffer != NULL);

   return PerformVirtualMemoryOperation(handle, ReadVirtualMemoryChunk, parameters, buffer, callContext);
}

VirtualMemoryStatus VirtualMemoryWrite(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                       const VirtualMemoryParameters *parameters,
                                       const void *buffer,
                                       const void *callContext)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(handle->IsInitialized == true);
   SDeviceAssert(buffer != NULL);

   return PerformVirtualMemoryOperation(handle, WriteVirtualMemoryChunk, parameters, buffer, callContext);
}
