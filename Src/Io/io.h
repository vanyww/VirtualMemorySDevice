#pragma once

#include "search.h"
#include "Chunks/Base/interface.h"

static SDevicePropertyStatus InvokeSpanOperation(
      ThisHandle                    *handle,
      IO_OPERATION_POINTER          (operation),
      const ThisOperationParameters *parameters,
      const void                    *context)
{
   SDevicePropertyStatus status;
   ThisSizeType size = parameters->AsAny.Size;

   if(size > 0)
   {
      ThisAddressType lastAddress, firstAddress = parameters->AsAny.Address;

      if(TRY_ADD_INT_CHECKED(firstAddress, size - 1, &lastAddress) && lastAddress <= GET_HIGHEST_ADDRESS(handle))
      {
         MemoryReference memoryReference = FindMemoryReference(handle, firstAddress);
         ChunkOperationParameters chunkParameters =
         {
            .AsAny =
            {
               .Data   = parameters->AsAny.Data,
               .Offset = memoryReference.Offset,
               .Size   = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size)
            }
         };

         status = operation(handle, memoryReference.Chunk, &chunkParameters, context);

         if(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size)
         {
            chunkParameters.AsAny.Offset = 0;

            do
            {
               memoryReference.Chunk++;
               size -= chunkParameters.AsAny.Size;
               chunkParameters.AsAny.Data += chunkParameters.AsAny.Size;
               chunkParameters.AsAny.Size = MIN(memoryReference.Chunk->Size, size);

               status = operation(handle, memoryReference.Chunk, &chunkParameters, context);
            }
            while(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size);
         }
      }
      else
      {
         SDeviceLogStatus(handle, VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS);
         status = SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR;
      }
   }
   else
   {
      status = SDEVICE_PROPERTY_STATUS_OK;
   }

   return status;
}
