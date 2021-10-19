#include "test_initialization.h"
#include "../../Device/test_device.h"
#include "../../Device/Mock/Assertation/mock_assert.h"

bool TestVirtualMemoryHandleInitialization(void)
{
   __SDEVICE_HANDLE(VirtualMemory) handle = CreateVirtualMemorySDevice();

   if(handle.IsInitialized != true)
      return false;

   if(WasAssertFailed() == true)
      return false;

   return true;
}
