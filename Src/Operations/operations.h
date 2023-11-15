#pragma once

#include "helpers.h"

#include <memory.h>

static ChunkStatusInternal PerformMemoryOperation(ThisHandle               *handle,
                                                  ChunkOperation            operation,
                                                  const OperationParameters parameters)
{
   ChunkStatusInternal status;
   SizeType size = parameters.AsCommon->Size;

   if(size > 0)
   {
      AddressType lastAddress,
                  firstAddress = parameters.AsCommon->Address;

      if(TRY_ADD_INT_CHECKED(firstAddress, size - 1, &lastAddress) && lastAddress <= handle->Runtime->HighestAddress)
      {
         MemoryReference memoryReference = FindMemoryReference(handle, firstAddress);
         ChunkOperationParameters chunkParameters =
         {
            .AsCommon =
            {
              .Data        = parameters.AsCommon->Data,
              .Offset      = memoryReference.Offset,
              .Size        = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size),
              .CallContext = parameters.AsCommon->CallContext
            }
         };

         status = operation(handle, memoryReference.Chunk, &chunkParameters);

         if(status == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK && size > chunkParameters.AsCommon.Size)
         {
            chunkParameters.AsCommon.Offset = 0;

            do
            {
               memoryReference.Chunk++;
               size -= chunkParameters.AsCommon.Size;
               chunkParameters.AsCommon.Data += chunkParameters.AsCommon.Size;
               chunkParameters.AsCommon.Size = MIN(memoryReference.Chunk->Size, size);

               status = operation(handle, memoryReference.Chunk, &chunkParameters);
            }
            while(status == VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK && size > chunkParameters.AsCommon.Size);
         }
      }
      else
      {
         SDeviceLogStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS);
         status = VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL;
      }
   }
   else
   {
      status = VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
   }

   return status;
}

static ChunkStatusInternal ReadChunk(ThisHandle                     *handle,
                                     const ChunkInternal            *chunk,
                                     const ChunkOperationParameters *parameters)
{
   ChunkStatusInternal status;

   if(chunk->Read != NULL)
   {
      status = chunk->Read(handle, chunk, &parameters->AsRead);

      SDeviceAssert(VIRTUAL_MEMORY_SDEVICE_IS_VALID_CHUNK_STATUS(status));

      if(status != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
         LogReadFailStatus(handle, FindChunkAddress(handle, chunk));
   }
   else
   {
      memset(parameters->AsRead.Data, VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE, parameters->AsRead.Size);
      status = VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
   }

   return status;
}

static ChunkStatusInternal WriteChunk(ThisHandle                     *handle,
                                      const ChunkInternal            *chunk,
                                      const ChunkOperationParameters *parameters)
{
   ChunkStatusInternal status;

   if(chunk->Write != NULL)
   {
      status = chunk->Write(handle, chunk, &parameters->AsWrite);

      SDeviceAssert(VIRTUAL_MEMORY_SDEVICE_IS_VALID_CHUNK_STATUS(status));

      if(status != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
         LogWriteFailStatus(handle, FindChunkAddress(handle, chunk));
   }
   else
   {
      status = VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
   }

   return status;
}
