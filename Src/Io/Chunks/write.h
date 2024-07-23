#include "Base/interface.h"
#include "../search.h"

typedef union
{
   ChunkOperationParametersBase AsBase;
   ThisChunkWriteParameters     AsThis;
} ChunkWriteOperationParameters;

IO_OPERATION_DECLARATION(Write, handle, chunk, parameters, context)
{
   if(chunk->Write)
   {
      const ThisChunkWriteParameters *_parameters =
            &((const ChunkWriteOperationParameters *)parameters)->AsThis;

      SDevicePropertyStatus status = chunk->Write(handle, chunk, _parameters, context);

      SDeviceAssert(SDEVICE_IS_VALID_PROPERTY_OPERATION_STATUS(status));

      if(status != SDEVICE_PROPERTY_STATUS_OK)
         LogWriteFailStatus(handle, FindChunkAddress(handle, chunk));

      return status;
   }

   return SDEVICE_PROPERTY_STATUS_OK;
}
