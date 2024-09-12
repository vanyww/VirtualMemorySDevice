#include "../../Mock/VirtualMemorySDevice/application.h"
#include "../../Mock/Errors/errors.h"

#include "unity_fixture.h"


TEST_GROUP(Read);

TEST_SETUP(Read)
{
   EraseChunksBuffers();
   AssertionMustBeFail(false);
}

TEST_TEAR_DOWN(Read) {}

TEST(Read, FirstChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, 0, sizeof(readData) }
   };

   WriteChunkBuffer(0, expectedData, sizeof(expectedData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LastChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT - 1), sizeof(readData) }
   };

   WriteChunkBuffer(CHUNKS_COUNT - 1, expectedData, sizeof(expectedData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, MiddleChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t expectedData[] = { 0x11 };
   uint8_t readData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE * (CHUNKS_COUNT / 2 - 1), sizeof(readData) }
   };

   WriteChunkBuffer(CHUNKS_COUNT/2 - 1, expectedData, sizeof(expectedData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, AddressInsideChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t fillingData[] = { [0 ... CHUNK_SIZE - 1] = 0x11 };
   uint8_t expectedData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11 };
   uint8_t readData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readData, CHUNK_SIZE / 2, sizeof(readData) }
   };


   WriteChunkBuffer(0, fillingData, sizeof(fillingData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSize)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

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


   WriteChunkBuffer(0, expectedData, sizeof(expectedData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, LargerThanOneChunkSizeWithAddressInsideChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

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


   WriteChunkBuffer(0, fillingData, sizeof(fillingData));
   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, parameters.AsRead.Data, sizeof(readData));
}

TEST(Read, WrongAddress)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   char *readDataStub[1];
   size_t address = SIZE_MAX;
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type   = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ,
      .AsRead = { readDataStub, address, sizeof(readDataStub) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
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
