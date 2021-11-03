#include "test_read.h"
#include "../../../Device/test_device.h"
#include "../../../Device/Mock/Chunks/mock_chunks.h"
#include "../../../Device/Mock/Assertation/mock_assert.h"
#include "../../../Device/Mock/RuntimeError/mock_handle_runtime_error.h"

#include <memory.h>

bool TestReadNormal(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle = { 0 };
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedReadData[] = { 0x11, 0x22, 0x33, 0x44 };
   MockChunksBuffers[0][0] = expectedReadData[0];
   MockChunksBuffers[0][1] = expectedReadData[1];
   MockChunksBuffers[1][0] = expectedReadData[2];
   MockChunksBuffers[1][1] = expectedReadData[3];
   uint8_t readData[2 * __MOCK_CHUNK_SIZE];

   VirtualMemorySDeviceRead(&handle, NULL, readData, 0, sizeof(readData));

   if(WasAssertFailed() == true)
      return false;

   if(WasRuntimeErrorRaised() == true)
      return false;

   if(memcmp(expectedReadData, readData, sizeof(readData)) != 0)
      return false;

   return true;
}

bool TestReadEmpty(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle = { 0 };
   CreateVirtualMemorySDevice(&handle);

   uint8_t expectedReadData[] = { 0x11, 0x22, 0x00, 0x00 };
   MockChunksBuffers[1][0] = expectedReadData[0];
   MockChunksBuffers[1][1] = expectedReadData[1];
   MockChunksBuffers[2][0] = 0xFF;
   MockChunksBuffers[2][1] = 0xFF;
   uint8_t readData[2 * __MOCK_CHUNK_SIZE];

   VirtualMemorySDeviceRead(&handle, NULL, readData, __MOCK_CHUNK_SIZE, sizeof(readData));

   if(WasAssertFailed() == true)
      return false;

   if(WasRuntimeErrorRaised() == true)
      return false;

   if(memcmp(expectedReadData, readData, sizeof(readData)) != 0)
      return false;

   return true;
}
