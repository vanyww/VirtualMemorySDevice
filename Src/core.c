#include "VirtualMemorySDevice/public.h"
#include "Operations/operations.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

SDEVICE_STRING_NAME_DEFINITION(VirtualMemory);

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context)
{
   SDeviceAssert(init != NULL);

   const ThisInitData *_init = init;

   SDeviceAssert(_init->Chunks != NULL || _init->ChunksCount == 0);

   ThisHandle *handle = SDeviceMalloc(sizeof(ThisHandle));
   handle->Header = (SDeviceHandleHeader)
   {
      .Context           = context,
      .OwnerHandle       = owner,
      .SDeviceStringName = SDEVICE_STRING_NAME(VirtualMemory),
      .LatestStatus      = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
      .Identifier        = identifier
   };
   handle->Init = *_init;

   size_t totalChunksSize = 0;
   for(size_t i = 0; i < _init->ChunksCount; i++)
   {
      size_t chunkSize = _init->Chunks[i].Size;

      SDeviceAssert(chunkSize != 0);
      SDeviceEvalAssert(!__builtin_add_overflow(totalChunksSize, chunkSize, &totalChunksSize));
   }

   handle->Runtime = (ThisRuntimeData)
   {
      .TotalChunksSize = totalChunksSize
   };

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer)
{
   SDeviceAssert(handlePointer != NULL);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(handle != NULL);

   SDeviceFree(handle);
   *_handlePointer = NULL;
}

bool VirtualMemorySDeviceTryRead(ThisHandle *handle, const ReadParameters *parameters)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(parameters != NULL);
   SDeviceAssert(parameters->Data != NULL || parameters->Size == 0);

   return TryPerformVirtualMemoryOperation(handle, ReadChunk, parameters);
}

bool VirtualMemorySDeviceTryWrite(ThisHandle *handle, const WriteParameters *parameters)
{
   SDeviceAssert(handle != NULL);
   SDeviceAssert(parameters != NULL);
   SDeviceAssert(parameters->Data != NULL || parameters->Size == 0);

   return TryPerformVirtualMemoryOperation(handle, WriteChunk, parameters);
}
