#include "Base/interface.h"
#include "../search.h"

#include <memory.h>

typedef union
{
   ChunkOperationParametersBase AsBase;
   ThisChunkGetParameters       AsThis;
} ChunkReadOperationParameters;

IO_OPERATION_DECLARATION(Get, handle, chunk, parameters, context)
{
   const ThisChunkGetParameters *_parameters =
         &((const ChunkReadOperationParameters *)parameters)->AsThis;

   if(chunk->Get)
   {
      SDevicePropertyStatus status = chunk->Get(handle, chunk, _parameters, context);

      SDeviceAssert(SDEVICE_IS_VALID_PROPERTY_OPERATION_STATUS(status));

      if(status != SDEVICE_PROPERTY_STATUS_OK)
         LogGetFailStatus(handle, FindChunkAddress(handle, chunk));

      return status;
   }

   memset(_parameters->Data, VIRTUAL_MEMORY_SDEVICE_FILLER_DATA_VALUE, _parameters->Size);

   return SDEVICE_PROPERTY_STATUS_OK;
}
