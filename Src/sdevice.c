#include "../Inc/VirtualMemorySDevice/core.h"

#include <stddef.h>

__SDEVICE_INITIALIZE_HANDLE_DECLARATION(VirtualMemory, handle)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(handle->Constant != NULL);
   SDeviceAssert(handle->IsInitialized == false);
   SDeviceAssert(handle->Constant->ChunksList != NULL);
   SDeviceAssert(handle->Constant->ChunksList->Count != 0);
   SDeviceAssert(handle->Constant->ChunksList->Chunks != NULL);

   VirtualMemoryBaseType address = handle->Constant->AddressingStart;

   for(VirtualMemoryBaseType i = 0; i < handle->Constant->ChunksList->Count; i++)
   {
      VirtualMemoryBaseType chunkBytesCount = handle->Constant->ChunksList->Chunks[i].BytesCount;

      if(chunkBytesCount == 0)
         continue;

      SDeviceAssert(__VIRTUAL_MEMORY_BASE_TYPE_MAX_VALUE - address >= chunkBytesCount - 1);
      address += chunkBytesCount;
   }

   SDeviceAssert(address != handle->Constant->AddressingStart);

   handle->Dynamic.AddressingEnd = address - 1;
   handle->IsInitialized = true;
}
