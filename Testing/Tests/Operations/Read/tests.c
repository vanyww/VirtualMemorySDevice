#include "../../Mock/SDevice/virtual_memory.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(VirtualMemory) *Handle;

TEST_GROUP(Read);

TEST_SETUP(Read)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks      = Chunks,
      .ChunksCount = CHUNKS_COUNT
   };

   Handle = SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL);

   VirtualMemoryMockEraseChunksBuffers();
}

TEST_TEAR_DOWN(Read)
{
   SDEVICE_DISPOSE_HANDLE(VirtualMemory)(Handle);
}

TEST(Read, FirstChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, 0, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, LastChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT - 1), sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(CHUNKS_COUNT - 1, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, MiddleChunk)
{
   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT / 2 - 1), sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(CHUNKS_COUNT/2 - 1, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, AddressInsideChunk)
{
   uint8_t fillingData[] = { [0 ... CHUNK_SIZE - 1] = 0x11 };
   uint8_t expectedData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11 };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, CHUNK_SIZE / 2, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, fillingData, sizeof(fillingData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSize)
{
   uint8_t expectedData[] =
   {
      [0 ... CHUNK_SIZE - 1] = 0x11,
      [CHUNK_SIZE ... 2 * CHUNK_SIZE - 1] = 0x22
   };
   uint8_t readData[sizeof(expectedData)];

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, 0, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, expectedData, sizeof(expectedData));

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSizeWithAddressInsideChunk)
{
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
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readData, CHUNK_SIZE / 2, sizeof(readData) }
   };

   VirtualMemoryMockWriteChunkBuffer(0, fillingData, sizeof(fillingData));\

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.Interface.AsRead.Data, sizeof(readData));
}

TEST(Read, WrongAddress)
{
   char *readDataStub[1];
   size_t address = SIZE_MAX;

   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .TypeIdx   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ,
      .Interface.AsRead = { readDataStub, address, sizeof(readDataStub) }
   };

   TEST_ASSERT_EQUAL(
         SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR,
         VirtualMemorySDeviceAccess(Handle, &parameters, NULL));
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
