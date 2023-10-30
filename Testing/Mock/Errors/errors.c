#include "errors.h"

#include "SDeviceCore/errors.h"

void SDeviceProcessAssertFail(void)
{
   for(;;);
}

void SDeviceProcessPanic(const void *_handle)
{
   for(;;);
}

void SDeviceProcessLogStatus(const void *_handle, const void *extras, size_t extrasSize)
{
   for(;;);
}
