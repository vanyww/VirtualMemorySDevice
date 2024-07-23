#pragma once

#include "search.h"
#include "Chunks/Base/interface.h"

typedef struct
{
   void           *Data;
   ThisAddressType Address;
   ThisSizeType    Size;
} OperationParametersBase;

static SDevicePropertyStatus PerformOperation(
      ThisHandle                    *handle,
      IO_OPERATION_POINTER          (operation),
      const OperationParametersBase *parameters,
      const void                    *context)
{
   SDevicePropertyStatus status;
   ThisSizeType size = parameters->Size;

   if(size > 0)
   {
      ThisAddressType lastAddress, firstAddress = parameters->Address;

      if(TRY_ADD_INT_CHECKED(firstAddress, size - 1, &lastAddress) && lastAddress <= GET_HIGHEST_ADDRESS(handle))
      {
         MemoryReference memoryReference = FindMemoryReference(handle, firstAddress);
         ChunkOperationParametersBase chunkParameters =
         {
            .Data   = parameters->Data,
            .Offset = memoryReference.Offset,
            .Size   = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size)
         };

         status = operation(handle, memoryReference.Chunk, &chunkParameters, context);

         if(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.Size)
         {
            chunkParameters.Offset = 0;

            do
            {
               memoryReference.Chunk++;
               size -= chunkParameters.Size;
               chunkParameters.Data += chunkParameters.Size;
               chunkParameters.Size = MIN(memoryReference.Chunk->Size, size);

               status = operation(handle, memoryReference.Chunk, &chunkParameters, context);
            }
            while(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.Size);
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
