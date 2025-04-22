#include "Base/interface.h"
#include "../search.h"

#include "SDeviceCore/errors.h"

IO_OPERATION_DECLARATION(Set, handle, chunk, operationParameters, callParameters)
{
   if(chunk->Set)
   {
      const ThisChunkSetParameters *_operationParameters = &operationParameters->AsSet;

      SDevicePropertyStatus status = chunk->Set(handle, chunk, _operationParameters, callParameters);

      SDeviceAssert(SDEVICE_IS_VALID_PROPERTY_OPERATION_STATUS(status));

      return status;
   }

   return SDEVICE_PROPERTY_STATUS_OK;
}
