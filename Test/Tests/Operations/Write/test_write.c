#include "test_write.h"
#include "../../../Device/test_device.h"
#include "../../../Device/Mock/Chunks/mock_chunks.h"
#include "../../../Device/Mock/Assertation/mock_assert.h"

#include <memory.h>

bool TestWriteNormal(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle;
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedWrittenData[] = { 0x11, 0x22, 0x33, 0x44 };
   uint8_t dataToWrite[] = { 0x11, 0x22, 0x33, 0x44 };

   VirtualMemorySDeviceWrite(&handle, NULL, dataToWrite, 0, sizeof(dataToWrite));

   if(WasAssertFailed() == true)
      return false;

   if(memcmp(expectedWrittenData, MockChunksBuffers[0], sizeof(dataToWrite)) != 0)
      return false;

   return true;
}

bool TestWriteEmpty(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle;
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedWrittenData[] = { 0x11, 0x22, 0x00, 0x00 };
   uint8_t dataToWrite[] = { 0x11, 0x22, 0x33, 0x44 };

   VirtualMemorySDeviceWrite(&handle, NULL, dataToWrite, __MOCK_CHUNK_SIZE, sizeof(dataToWrite));

   if(WasAssertFailed() == true)
      return false;

   if(memcmp(expectedWrittenData, MockChunksBuffers[1], sizeof(dataToWrite)) != 0)
      return false;

   return true;
}
