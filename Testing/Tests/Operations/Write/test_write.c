#include "test_write.h"
#include "../../../Mock/Chunks/mock_chunks.h"
#include "../../../Mock/Errors/errors.h"

#include <memory.h>

bool TestWriteNormal(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init = { MockChunks, MockChunksCount };
      __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory)))) SDEVICE_HANDLE(VirtualMemory) *handle =
               SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);

   uint8_t expectedData[] = { 0x11, 0x22, 0x33, 0x44 };
   uint8_t writeData[] = { 0x11, 0x22, 0x33, 0x44 };

   const VirtualMemorySDeviceWriteParameters parameters = { writeData, NULL, 0, sizeof(writeData) };

   if(VirtualMemorySDeviceWrite(handle, &parameters) != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      return false;

   if(memcmp(expectedData, MockChunksBuffers[0], sizeof(writeData)) != 0)
      return false;

   return true;
}

bool TestWriteEmpty(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init = { MockChunks, MockChunksCount };
      __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory)))) SDEVICE_HANDLE(VirtualMemory) *handle =
               SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);

   uint8_t expectedData[] = { 0x11, 0x22, 0x00, 0x00 };
   uint8_t writeData[] = { 0x11, 0x22, 0x33, 0x44 };

   MockChunksBuffers[2][0] = 0x00;
   MockChunksBuffers[2][1] = 0x00;

   const VirtualMemorySDeviceWriteParameters parameters = { writeData, NULL, 2, sizeof(writeData) };

   if(VirtualMemorySDeviceWrite(handle, &parameters) != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      return false;

   if(memcmp(expectedData, MockChunksBuffers[1], sizeof(writeData)) != 0)
      return false;

   return true;
}
