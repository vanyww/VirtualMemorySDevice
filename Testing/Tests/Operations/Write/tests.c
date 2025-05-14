#include "../../Mock/SDevice/virtual_memory.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(VirtualMemory) *Handle;

TEST_GROUP(Write);

TEST_SETUP(Write)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks      = Chunks,
      .ChunksCount = CHUNKS_COUNT
   };

   Handle = SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL);

   VirtualMemoryMockEraseChunksBuffers();
}

TEST_TEAR_DOWN(Write)
{
   SDEVICE_DISPOSE_HANDLE(VirtualMemory)(Handle);
}

TEST(Write, FirstChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t writeData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, 0, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(0, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, LastChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t writeData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, CHUNK_SIZE * (CHUNKS_COUNT - 1), sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(CHUNKS_COUNT - 1, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, MiddleChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t writeData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, CHUNK_SIZE * (CHUNKS_COUNT / 2 - 1), sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(CHUNKS_COUNT / 2 - 1, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, AddressInsideChunk)
{
   uint8_t expectedData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x00, [CHUNK_SIZE/2 ... CHUNK_SIZE - 1] = 0x11 };
   uint8_t chunkData[sizeof(expectedData)];
   uint8_t fillingData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11 };

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { fillingData, CHUNK_SIZE / 2, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(0, chunkData, sizeof(chunkData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, chunkData, sizeof(expectedData));
}

TEST(Write, LargerThanOneChunkSize)
{
   uint8_t expectedData[] = { [0 ... CHUNK_SIZE - 1] = 0x11, [CHUNK_SIZE ... 2*CHUNK_SIZE - 1] = 0x22 };
   uint8_t writeData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, 0, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(0, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, LargerThanOneChunkSizeWithAddressInsideChunk)
{
   uint8_t fillingData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11, [CHUNK_SIZE/2 ... CHUNK_SIZE - 1] = 0x22 };
   uint8_t expectedData[] =
   {
      [0 ... CHUNK_SIZE/2 - 1] = 0x00,
      [CHUNK_SIZE/2 ... CHUNK_SIZE - 1] = 0x11,
      [CHUNK_SIZE ... 3*CHUNK_SIZE/2 - 1] = 0x22,
      [3*CHUNK_SIZE/2 ... 2*CHUNK_SIZE - 1] = 0x00
   };
   uint8_t chunksData [sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { fillingData, CHUNK_SIZE / 2, sizeof(fillingData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
   VirtualMemoryMockReadChunkBuffer(0, chunksData, sizeof(chunksData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, chunksData, sizeof(expectedData));
}

TEST(Write, WrongAddress)
{
   char *writeDataStub[1];
   size_t address = SIZE_MAX;

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { writeDataStub, address, sizeof(writeDataStub) }
   };

   TEST_ASSERT_EQUAL(
         SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR,
         VirtualMemorySDeviceInvokeOperation(Handle, &parameters, NULL));
}

TEST_GROUP_RUNNER(Write)
{
   RUN_TEST_CASE(Write, FirstChunk);
   RUN_TEST_CASE(Write, LastChunk);
   RUN_TEST_CASE(Write, MiddleChunk);
   RUN_TEST_CASE(Write, AddressInsideChunk);
   RUN_TEST_CASE(Write, LargerThanOneChunkSize);
   RUN_TEST_CASE(Write, LargerThanOneChunkSizeWithAddressInsideChunk);
   RUN_TEST_CASE(Write, WrongAddress);
}
