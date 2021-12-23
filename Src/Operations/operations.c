#include "operations.h"

#include <memory.h>

VirtualMemorySDeviceStatus VirtualMemoryTryReadChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                     void *data,
                                                     const VirtualMemorySDeviceChunk *chunk,
                                                     VirtualMemorySDeviceFunctionParameters *parameters)
{
   if(chunk->ReadFunction != NULL)
      return chunk->ReadFunction(handle, parameters, data, chunk->Context);

   memset(data, __VIRTUAL_MEMORY_SDEVICE_MOCK_VALUE, parameters->BytesCount);

   return VIRTUAL_MEMORY_SDEVICE_STATUS_OK;
}

VirtualMemorySDeviceStatus VirtualMemoryTryWriteChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                      void *data,
                                                      const VirtualMemorySDeviceChunk *chunk,
                                                      VirtualMemorySDeviceFunctionParameters *parameters)
{
   if(chunk->WriteFunction != NULL)
      return (VirtualMemorySDeviceStatus)chunk->WriteFunction(handle, parameters, data, chunk->Context);

   return VIRTUAL_MEMORY_SDEVICE_STATUS_OK;
}
