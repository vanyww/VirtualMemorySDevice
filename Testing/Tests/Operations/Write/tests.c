#include "../../Mock/VirtualMemorySDevice/application.h"
#include "../../Mock/Errors/errors.h"

#include "unity_fixture.h"


TEST_GROUP(Write);

TEST_SETUP(Write)
{
   EraseChunksBuffers();
   AssertionMustBeFail(false);
}

TEST_TEAR_DOWN(Write) {}

TEST(Write, FirstChunk)
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
   uint8_t writeData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, 0, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(0, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, LastChunk)
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
   uint8_t writeData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, CHUNK_SIZE * (CHUNKS_COUNT - 1), sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(CHUNKS_COUNT - 1, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, MiddleChunk)
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
   uint8_t writeData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, CHUNK_SIZE * (CHUNKS_COUNT / 2 - 1), sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(CHUNKS_COUNT / 2 - 1, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, AddressInsideChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t expectedData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x00, [CHUNK_SIZE/2 ... CHUNK_SIZE - 1] = 0x11 };
   uint8_t chunkData[sizeof(expectedData)];
   uint8_t fillingData[] = { [0 ... CHUNK_SIZE/2 - 1] = 0x11 };
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { fillingData, CHUNK_SIZE / 2, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(0, chunkData, sizeof(chunkData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, chunkData, sizeof(expectedData));
}

TEST(Write, LargerThanOneChunkSize)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   uint8_t expectedData[] = { [0 ... CHUNK_SIZE - 1] = 0x11, [CHUNK_SIZE ... 2*CHUNK_SIZE - 1] = 0x22 };
   uint8_t writeData[sizeof(expectedData)];
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { expectedData, 0, sizeof(expectedData) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(0, writeData, sizeof(writeData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, writeData, sizeof(expectedData));
}

TEST(Write, LargerThanOneChunkSizeWithAddressInsideChunk)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

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

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_OK, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
   ReadChunkBuffer(0, chunksData, sizeof(chunksData));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, chunksData, sizeof(expectedData));
}

TEST(Write, WrongAddress)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
   SetAssertFailHandle(handle);

   char *writeDataStub[1];
   size_t address = SIZE_MAX;
   const VirtualMemorySDeviceOperationParameters parameters =
   {
      .Type    = VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE,
      .AsWrite = { writeDataStub, address, sizeof(writeDataStub) }
   };

   TEST_ASSERT_EQUAL(SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR, VirtualMemorySDeviceInvokeOperation(handle, &parameters, NULL));
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
