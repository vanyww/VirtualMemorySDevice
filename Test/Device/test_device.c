#include "test_device.h"
#include "Mock/Chunks/mock_chunks.h"

#include <memory.h>

__SDEVICE_HANDLE(VirtualMemory) CreateVirtualMemorySDevice(void)
{
   memset(MockChunksBuffers, 0, sizeof(MockChunksBuffers));

   __SDEVICE_HANDLE(VirtualMemory) handle =
   {
      {
         .Chunks = MockChunks,
         .ChunksCount = MockChunksCount,
         .AddressingStart = 0x00,
         .Context = NULL
      }
   };

   __SDEVICE_INITIALIZE_HANDLE(VirtualMemory)(&handle);

   return handle;
}
