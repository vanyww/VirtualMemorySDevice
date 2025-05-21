#include "get.h"

#include <memory.h>

CHUNK_OPERATION_DECLARATION(Get, this, chunk, operationParameters, callParameters)
{
   const VirtualMemorySDeviceChunkGetParameters *_operationParameters =
         &operationParameters->AsGet;

   if(chunk->Get)
      return chunk->Get(this, chunk, _operationParameters, callParameters);

   memset(
         _operationParameters->Data,
         VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE,
         _operationParameters->Size);

   return SDEVICE_PROPERTY_STATUS_OK;
}
