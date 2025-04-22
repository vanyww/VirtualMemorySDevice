#include "../../Mock/SDevice/virtual_memory.h"

#include "unity_fixture.h"

TEST_GROUP(Read);

TEST_SETUP(Read)
{
   VirtualMemoryMockEraseChunksBuffers();
}

TEST_TEAR_DOWN(Read) {}

TEST(Read, FirstChunk)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, 0, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LastChunk)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT - 1), sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(CHUNKS_COUNT - 1, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, MiddleChunk)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT / 2 - 1), sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(CHUNKS_COUNT/2 - 1, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, AddressInsideChunk)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t fillingData[] = { [0 ... CHUNK_SIZE - 1] = 0x11 };
   uint8_t expectedData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE / 2, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, fillingData, sizeof(fillingData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSize)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t expectedData[] =
   {
      [0 ... CHUNK_SIZE - 1] = 0x11,
      [CHUNK_SIZE ... 2 * CHUNK_SIZE - 1] = 0x22
   };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, 0, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSizeWithAddressInsideChunk)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   uint8_t fillingData[] =
   {
      [0 ... CHUNK_SIZE - 1] = 0x11,
      [CHUNK_SIZE ... 2 * CHUNK_SIZE - 1] = 0x22
   };
   uint8_t expectedData[] =
   {
         [0 ... CHUNK_SIZE / 2 - 1] = 0x11,
         [CHUNK_SIZE / 2 ... CHUNK_SIZE - 1] = 0x22
   };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE / 2, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, fillingData, sizeof(fillingData));\

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, WrongAddress)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle = VirtualMemoryMockCreateInstance();

   char *readDataStub[1];
   size_t address = SIZE_MAX;

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readDataStub, address, sizeof(readDataStub) }
   };

   TEST_ASSERT_EQUAL(
         SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR,
         VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
}

TEST_GROUP_RUNNER(Read)
{
   RUN_TEST_CASE(Read, FirstChunk);
   RUN_TEST_CASE(Read, LastChunk);
   RUN_TEST_CASE(Read, MiddleChunk);
   RUN_TEST_CASE(Read, AddressInsideChunk);
   RUN_TEST_CASE(Read, LargerThanOneChunkSize);
   RUN_TEST_CASE(Read, LargerThanOneChunkSizeWithAddressInsideChunk);
   RUN_TEST_CASE(Read, WrongAddress);
}
