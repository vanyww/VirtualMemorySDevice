#include "test_device.h"
#include "Mock/Chunks/mock_chunks.h"

#include <memory.h>

__SDEVICE_HANDLE(VirtualMemory) CreateVirtualMemorySDevice(void)
{
   memset(MockChunksBuffers, 0, sizeof(MockChunksBuffers));

   __SDEVICE_HANDLE(VirtualMemory) handle =
   {
      .Init = (__SDEVICE_INIT_DATA(VirtualMemory))
      {
         .ChunksList = &ChunksList,
         .AddressingStart = 0x00
      }
   };
   __SDEVICE_INITIALIZE_HANDLE(VirtualMemory)(&handle);

   return handle;
}
