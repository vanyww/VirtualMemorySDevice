#include "set.h"

CHUNK_OPERATION_DECLARATION(Set, this, chunk, operationParameters, callParameters)
{
   const VirtualMemorySDeviceChunkSetParameters *_operationParameters =
         &operationParameters->AsSet;

   if(chunk->Set)
      return chunk->Set(this, chunk, _operationParameters, callParameters);

   return SDEVICE_PROPERTY_STATUS_OK;
}
