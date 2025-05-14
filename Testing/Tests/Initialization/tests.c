#include "../../Mock/SDevice/virtual_memory.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(VirtualMemory) *Handle;

TEST_GROUP(Initialization);

TEST_SETUP(Initialization)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks      = Chunks,
      .ChunksCount = CHUNKS_COUNT
   };

   Handle = SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL);

   VirtualMemoryMockEraseChunksBuffers();
}

TEST_TEAR_DOWN(Initialization)
{
   SDEVICE_DISPOSE_HANDLE(VirtualMemory)(Handle);
}

TEST(Initialization, VirtualMemory)
{
   TEST_ASSERT_NOT_NULL(Handle);
}

TEST_GROUP_RUNNER(Initialization)
{
   RUN_TEST_CASE(Initialization, VirtualMemory)
}
