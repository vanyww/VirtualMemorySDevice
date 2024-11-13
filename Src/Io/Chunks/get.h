#include "Base/interface.h"
#include "../search.h"

#include <memory.h>

IO_OPERATION_DECLARATION(Get, handle, chunk, operationParameters, callParameters)
{
   const ThisChunkGetParameters *_operationParameters = &operationParameters->AsGet;

   if(chunk->Get)
   {
      SDevicePropertyStatus status = chunk->Get(handle, chunk, _operationParameters, callParameters);

      SDeviceAssert(SDEVICE_IS_VALID_PROPERTY_OPERATION_STATUS(status));

      if(status != SDEVICE_PROPERTY_STATUS_OK)
         LogChunkGetFailStatus(handle, FindChunkAddress(handle, chunk));

      return status;
   }

   memset(_operationParameters->Data, VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE, _operationParameters->Size);

   return SDEVICE_PROPERTY_STATUS_OK;
}
