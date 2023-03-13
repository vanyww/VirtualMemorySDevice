#include "data_types.h"

#include "SDeviceCore/common.h"
#include "SDeviceCore/errors.h"

#include <memory.h>

#ifndef VIRTUAL_MEMORY_SDEVICE_MOCK_VALUE
#define VIRTUAL_MEMORY_SDEVICE_MOCK_VALUE 0x00
#endif

static bool TryGetVirtualMemoryPointer(ThisHandle *handle, uintptr_t address, VirtualMemoryPointer *pointer)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(pointer != NULL);

   if(address >= handle->Runtime.TotalChunksSize)
      return false;

   uintptr_t nextChunkAddress = 0;
   for(size_t i = 0; i < handle->Init.ChunksCount; i++)
   {
      const Chunk *chunk = &handle->Init.Chunks[i];
      size_t chunkSize = chunk->Size;

      nextChunkAddress += chunkSize;

      if(address < nextChunkAddress)
      {
         *pointer = (VirtualMemoryPointer)
         {
            .Chunk = chunk,
            .Offset = address - (nextChunkAddress - chunkSize)
         };

         return true;
      }
   }

   return false;
}

static bool TryPerformVirtualMemoryOperation(ThisHandle *handle,
                                             ChunkOperation operation,
                                             const OperationParameters parameters)
{
   SDeviceDebugAssert(handle != NULL);

   VirtualMemoryPointer memoryPointer;
   if(!TryGetVirtualMemoryPointer(handle, parameters.AsCommon->Address, &memoryPointer))
   {
      SDeviceLogStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_CHUNK_ADDRESS);
      return false;
   }

   size_t leftToRead = parameters.AsCommon->Size;
   ChunkOperationParameters chunkParameters =
   {
      .AsCommon =
      {
        .DataPointer = parameters.AsCommon->DataPointer,
        .CallContext = parameters.AsCommon->CallContext,
        .Offset = memoryPointer.Offset,
        .Size = MIN(memoryPointer.Chunk->Size - memoryPointer.Offset, leftToRead)
      }
   };

   while(leftToRead > 0)
   {
      chunkParameters.AsCommon.ChunkContext = memoryPointer.Chunk->Context;
      ChunkStatus status = operation(handle, memoryPointer.Chunk, &chunkParameters);
      if(status != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      {
         SDeviceLogStatus(handle, status);
         return false;
      }

      memoryPointer.Chunk++;
      leftToRead -= chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.DataPointer += chunkParameters.AsCommon.Size;
      chunkParameters.AsCommon.Size = MIN(memoryPointer.Chunk->Size, leftToRead);
      chunkParameters.AsCommon.Offset = 0;
   }

   return true;
}

static ChunkStatus TryReadChunk(ThisHandle *handle, const Chunk *chunk, const ChunkOperationParameters *parameters)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(chunk != NULL);

   if(chunk->Read != NULL)
   {
      SDeviceDebugAssert(parameters != NULL);

      return chunk->Read(handle, &parameters->AsRead);
   }

   memset(parameters->AsRead.Data, VIRTUAL_MEMORY_SDEVICE_MOCK_VALUE, parameters->AsRead.Size);

   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

static ChunkStatus TryWriteChunk(ThisHandle *handle, const Chunk *chunk, const ChunkOperationParameters *parameters)
{
   SDeviceDebugAssert(handle != NULL);
   SDeviceDebugAssert(chunk != NULL);

   if(chunk->Write != NULL)
   {
      SDeviceDebugAssert(parameters != NULL);

      return chunk->Write(handle, &parameters->AsWrite);
   }

   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}
