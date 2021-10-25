#include "test_device.h"
#include "Mock/Chunks/mock_chunks.h"

#include <memory.h>

void CreateVirtualMemorySDevice(__SDEVICE_HANDLE(VirtualMemory) *handle)
{
   memset(MockChunksBuffers, 0, sizeof(MockChunksBuffers));

   handle->Constant = &ConstandData;
   __SDEVICE_INITIALIZE_HANDLE(VirtualMemory)(handle);
}
