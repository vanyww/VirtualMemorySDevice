#include "data_types.h"

#include "SDeviceCore/common.h"
#include "SDeviceCore/errors.h"

#include <memory.h>

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
static VirtualMemoryReference GetVirtualMemoryReference(ThisHandle *handle, uintptr_t address)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(address < handle->Runtime.TotalChunksSize);

   size_t leftIdx = 0;
   size_t rightIdx = handle->Runtime.TotalChunksSize - 1;

   while(rightIdx - leftIdx > 1)
   {
      size_t midIdx = (leftIdx + rightIdx)/2;
      uintptr_t chunkAddress = handle->Runtime.ChunksAddresses[midIdx];

      if(address > chunkAddress)
      {
         leftIdx = midIdx;
      }
      else if (address < chunkAddress)
      {
         rightIdx = midIdx;
      }
      else
      {
         return (VirtualMemoryReference)
         {
            .Chunk = &handle->Init.Chunks[midIdx],
            .Offset = 0
         };
      }
   }

   size_t resultIdx = (address > handle->Runtime.ChunksAddresses[rightIdx]) ? rightIdx : leftIdx;
   return (VirtualMemoryReference)
   {
      .Chunk = &handle->Init.Chunks[resultIdx],
      .Offset = address - handle->Runtime.ChunksAddresses[resultIdx]
   };
}
#else
static VirtualMemoryReference GetVirtualMemoryReference(ThisHandle *handle, uintptr_t address)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(address < handle->Runtime.TotalChunksSize);

   const Chunk *currentChunk = &handle->Init.Chunks[0];
   uintptr_t lastAddress = currentChunk->Size - 1;

   while(address > lastAddress)
   {
      currentChunk++;
      lastAddress += currentChunk->Size;
   };

   return (VirtualMemoryReference)
   {
      .Chunk = currentChunk,
      .Offset = currentChunk->Size - ((lastAddress - address) + 1)
   };
}
#endif

static bool TryPerformVirtualMemoryOperation(ThisHandle *handle,
                                             ChunkOperation operation,
                                             const OperationParameters parameters)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(operation != NULL);
   SDeviceDebugAssert(parameters.AsCommon != NULL);

   size_t size = parameters.AsCommon->Size;
   uintptr_t address = parameters.AsCommon->Address;

   if(size == 0)
      return true;

   uintptr_t lastAddress;
   if(__builtin_add_overflow(address, size - 1, &lastAddress) || lastAddress >= handle->Runtime.TotalChunksSize)
   {
      SDeviceLogStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS_OR_SIZE);
      return false;
   }

   VirtualMemoryReference memoryReference = GetVirtualMemoryReference(handle, address);
   ChunkOperationParameters chunkParameters =
   {
      .AsCommon =
      {
        .DataPointer = parameters.AsCommon->DataPointer,
        .Size        = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size),
        .Offset      = memoryReference.Offset,
        .CallContext = parameters.AsCommon->CallContext
      }
   };

   while(size > 0)
   {
      ChunkStatus status = operation(handle, memoryReference.Chunk, &chunkParameters);
      if(status != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      {
         SDeviceLogStatus(handle, status);
         return false;
      }

      size -= chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.DataPointer += chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.Size = MIN(memoryReference.Chunk->Size, size);
      chunkParameters.AsCommon.Offset = 0;
      memoryReference.Chunk++;
   }

   return true;
}

static ChunkStatus ReadChunk(ThisHandle *handle, const Chunk *chunk, const ChunkOperationParameters *parameters)
{
   SDeviceDebugAssert(chunk != NULL);
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(parameters != NULL);

   if(chunk->Read != NULL)
      return chunk->Read(handle, chunk, &parameters->AsRead);

   memset(parameters->AsRead.Data, VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE, parameters->AsRead.Size);

   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

static ChunkStatus WriteChunk(ThisHandle *handle, const Chunk *chunk, const ChunkOperationParameters *parameters)
{
   SDeviceDebugAssert(chunk != NULL);
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(parameters != NULL);

   if(chunk->Write != NULL)
      return chunk->Write(handle, chunk, &parameters->AsWrite);

   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}
