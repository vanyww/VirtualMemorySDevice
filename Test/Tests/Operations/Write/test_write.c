#include "test_write.h"
#include "../../../Device/test_device.h"
#include "../../../Device/Mock/Chunks/mock_chunks.h"
#include "../../../Device/Mock/Assertation/mock_assert.h"
#include "../../../Device/Mock/RuntimeError/mock_handle_runtime_error.h"

#include <memory.h>

bool TestWriteNormal(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle = { 0 };
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedData[] = { 0x11, 0x22, 0x33, 0x44 };
   uint8_t writeData[] = { 0x11, 0x22, 0x33, 0x44 };

   VirtualMemoryStatus status =
            VirtualMemoryWrite(&handle, &(VirtualMemoryParameters){ 0, sizeof(writeData) }, writeData, NULL);

   if(status != VIRTUAL_MEMORY_STATUS_OK)
      return false;

   if(WasAssertFailed() == true)
      return false;

   if(WasRuntimeErrorRaised() == true)
      return false;

   if(memcmp(expectedData, MockChunksBuffers[0], sizeof(writeData)) != 0)
      return false;

   return true;
}

bool TestWriteEmpty(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle = { 0 };
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedData[] = { 0x11, 0x22, 0x00, 0x00 };
   uint8_t writeData[] = { 0x11, 0x22, 0x33, 0x44 };

   VirtualMemoryStatus status =
            VirtualMemoryWrite(&handle, &(VirtualMemoryParameters){ 2, sizeof(writeData) }, writeData, NULL);

   if(status != VIRTUAL_MEMORY_STATUS_OK)
      return false;

   if(WasAssertFailed() == true)
      return false;

   if(WasRuntimeErrorRaised() == true)
      return false;

   if(memcmp(expectedData, MockChunksBuffers[1], sizeof(writeData)) != 0)
      return false;

   return true;
}
