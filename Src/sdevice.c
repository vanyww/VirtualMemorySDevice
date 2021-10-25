#include "../Inc/VirtualMemorySDevice/core.h"

#include <stddef.h>

__SDEVICE_INITIALIZE_HANDLE_DECLARATION(VirtualMemory, handle)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(handle->Constant != NULL);
   SDeviceAssert(handle->IsInitialized == false);
   SDeviceAssert(handle->Constant->Chunks != NULL);

#ifdef __SDEVICE_ASSERT
   VirtualMemorySDeviceBaseType address = handle->Constant->AddressingStart;

   for(size_t i = 0; i < handle->Constant->ChunksCount; i++)
   {
      VirtualMemorySDeviceBaseType chunkBytesCount = handle->Constant->Chunks[i].BytesCount;

      if(chunkBytesCount == 0)
         continue;

      SDeviceAssert(__VIRTUAL_MEMORY_SDEVICE_BASE_TYPE_MAX_VALUE - address >= chunkBytesCount - 1);
      address += chunkBytesCount;
   }
#endif

   handle->IsInitialized = true;
}
