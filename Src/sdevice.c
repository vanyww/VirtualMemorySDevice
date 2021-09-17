#include "VirtualMemorySDevice/core.h"

#include <stddef.h>

__SDEVICE_INITIALIZE_INTERNALS_DECLARATION(VirtualMemory, handle)
{
   SDeviceAssert(handle->Constant->Chunks != NULL);

#ifdef __SDEVICE_ASSERT
   VirtualMemorySDeviceBaseType address = handle->Constant->AddressingStart - 1;

   for(size_t i = 0; i < handle->Constant->ChunksCount; i++)
   {
      SDeviceAssert(__VIRTUAL_MEMORY_SDEVICE_BASE_TYPE_MAX_VALUE - address <= handle->Constant->Chunks[i].BytesCount);
      address += handle->Constant->Chunks[i].BytesCount;
   }
#endif

   return true;
}
