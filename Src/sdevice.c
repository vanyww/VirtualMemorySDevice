#include "../Inc/VirtualMemorySDevice/core.h"

__SDEVICE_INITIALIZE_HANDLE_DECLARATION(VirtualMemory, handle)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(handle->IsInitialized == false);
   SDeviceAssert(handle->Init.ChunksList != NULL);
   SDeviceAssert(handle->Init.ChunksList->Count != 0);
   SDeviceAssert(handle->Init.ChunksList->Chunks != NULL);

   VirtualMemoryBaseType address = handle->Init.AddressingStart;

   for(VirtualMemoryBaseType i = 0; i < handle->Init.ChunksList->Count; i++)
   {
      VirtualMemoryBaseType chunkBytesCount = handle->Init.ChunksList->Chunks[i].BytesCount;

      if(chunkBytesCount == 0)
         continue;

      SDeviceAssert(__VIRTUAL_MEMORY_BASE_TYPE_MAX_VALUE - address >= chunkBytesCount - 1);
      address += chunkBytesCount;
   }

   SDeviceAssert(address != handle->Init.AddressingStart);

   handle->Runtime.AddressingEnd = address - 1;
   handle->IsInitialized = true;
}
