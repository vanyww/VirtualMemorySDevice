#include "../../Mock/SDevice/virtual_memory.h"

#include "unity_fixture.h"

TEST_GROUP(Initialization);

TEST_SETUP(Initialization)
{
   VirtualMemoryMockEraseChunksBuffers();
}

TEST_TEAR_DOWN(Initialization) {}

TEST(Initialization, VirtualMemory)
{
   _cleanup SDEVICE_HANDLE(VirtualMemory) *handle =
         VirtualMemoryMockCreateInstance();

   TEST_ASSERT_NOT_NULL(handle);
}

TEST_GROUP_RUNNER(Initialization)
{
   RUN_TEST_CASE(Initialization, VirtualMemory)
}
