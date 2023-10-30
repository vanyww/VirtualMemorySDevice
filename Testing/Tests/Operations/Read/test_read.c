#include "test_read.h"
#include "../../../Mock/Chunks/mock_chunks.h"
#include "../../../Mock/Errors/errors.h"

#include <memory.h>

bool TestReadNormal(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init = { MockChunks, MockChunksCount };
      __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory)))) SDEVICE_HANDLE(VirtualMemory) *handle =
               SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);

   uint8_t expectedData[] = { 0x11, 0x22, 0x33, 0x44 };
   MockChunksBuffers[0][0] = expectedData[0];
   MockChunksBuffers[0][1] = expectedData[1];
   MockChunksBuffers[1][0] = expectedData[2];
   MockChunksBuffers[1][1] = expectedData[3];
   uint8_t readData[2 * __MOCK_CHUNK_SIZE];

   const VirtualMemorySDeviceReadParameters parameters = { readData, NULL, 0, sizeof(readData) };

   if(VirtualMemorySDeviceRead(handle, &parameters) != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      return false;

   if(memcmp(expectedData, readData, sizeof(readData)) != 0)
      return false;

   return true;
}

bool TestReadEmpty(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init = { MockChunks, MockChunksCount };
      __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory)))) SDEVICE_HANDLE(VirtualMemory) *handle =
               SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);

   uint8_t expectedData[] = { 0x11, 0x22, 0x00, 0x00 };
   MockChunksBuffers[1][0] = expectedData[0];
   MockChunksBuffers[1][1] = expectedData[1];
   MockChunksBuffers[2][0] = 0xFF;
   MockChunksBuffers[2][1] = 0xFF;
   uint8_t readData[2 * __MOCK_CHUNK_SIZE];

   const VirtualMemorySDeviceReadParameters parameters = { readData, NULL, 2, sizeof(readData) };

   if(VirtualMemorySDeviceRead(handle, &parameters) != VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK)
      return false;

   if(memcmp(expectedData, readData, sizeof(readData)) != 0)
      return false;

   return true;
}
