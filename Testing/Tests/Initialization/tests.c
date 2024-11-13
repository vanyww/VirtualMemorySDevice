#include "../../Mock/VirtualMemorySDevice/application.h"
#include "../../Mock/Errors/errors.h"

#include "unity_fixture.h"


TEST_GROUP(Initialization);

TEST_SETUP(Initialization)
{
   EraseChunksBuffers();
   AssertionMustBeFail(false);
}

TEST_TEAR_DOWN(Initialization) {}

TEST(Initialization, HandleInitialization)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = GetCunks(),
      .ChunksCount = CHUNKS_COUNT
   };

   VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE SDEVICE_HANDLE(VirtualMemory) *handle =
         SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);
}

TEST_GROUP_RUNNER(Initialization)
{
   RUN_TEST_CASE(Initialization, HandleInitialization)
}
