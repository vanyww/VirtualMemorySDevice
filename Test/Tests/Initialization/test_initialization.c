#include "test_initialization.h"
#include "../../Mock/Chunks/mock_chunks.h"
#include "../../Mock/Errors/errors.h"

bool TestVirtualMemoryHandleInitialization(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init = { MockChunks, MockChunksCount };
      __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory)))) SDEVICE_HANDLE(VirtualMemory) *handle =
               SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL, 0, NULL);

   if(WasAssertFailed() == true)
      return false;

   if(WasExceptionThrowed() == true)
      return false;

   if(WasStatusLogged() == true)
      return false;

   return true;
}
