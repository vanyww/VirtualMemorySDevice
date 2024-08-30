#include "Base/interface.h"
#include "../search.h"

typedef union
{
   ChunkOperationParametersBase AsBase;
   ThisChunkSetParameters       AsThis;
} ChunkWriteOperationParameters;

IO_OPERATION_DECLARATION(Set, handle, chunk, parameters, context)
{
   if(chunk->Set)
   {
      const ThisChunkSetParameters *_parameters =
            &((const ChunkWriteOperationParameters *)parameters)->AsThis;

      SDevicePropertyStatus status = chunk->Set(handle, chunk, _parameters, context);

      SDeviceAssert(SDEVICE_IS_VALID_PROPERTY_OPERATION_STATUS(status));

      if(status != SDEVICE_PROPERTY_STATUS_OK)
         LogSetFailStatus(handle, FindChunkAddress(handle, chunk));

      return status;
   }

   return SDEVICE_PROPERTY_STATUS_OK;
}
