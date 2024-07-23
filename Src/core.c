#include "private.h"
#include "Io/io.h"
#include "Io/Chunks/read.h"
#include "Io/Chunks/write.h"

#include "SDeviceCore/heap.h"

SDEVICE_IDENTITY_BLOCK_DEFINITION(
      VirtualMemory,
      ((const SDeviceUuid)
      {
         .High = VIRTUAL_MEMORY_SDEVICE_UUID_HIGH,
         .Low  = VIRTUAL_MEMORY_SDEVICE_UUID_LOW
      }),
      ((const SDeviceVersion)
      {
         .Major = VIRTUAL_MEMORY_SDEVICE_VERSION_MAJOR,
         .Minor = VIRTUAL_MEMORY_SDEVICE_VERSION_MINOR,
         .Patch = VIRTUAL_MEMORY_SDEVICE_VERSION_PATCH
      }));

typedef union
{
   OperationParametersBase AsBase;
   ThisReadParameters      AsThis;
} ReadOperationParameters;

typedef union
{
   OperationParametersBase AsBase;
   ThisWriteParameters     AsThis;
} WriteOperationParameters;

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context)
{
   SDeviceAssert(init);

   const ThisInitData *_init = init;

   SDeviceAssert(_init->Chunks);

   SDeviceAssert(_init->ChunksCount > 0);

#if SDEVICE_USE_ASSERT
   for(size_t i = 0; i < _init->ChunksCount; i++)
      SDeviceAssert(_init->Chunks[i].Size > 0);
#endif

   ThisHandle *instance = SDeviceAllocateHandle(sizeof(*instance->Init), sizeof(*instance->Runtime));

   instance->Header = (SDeviceHandleHeader)
   {
      .Context       = context,
      .OwnerHandle   = owner,
      .IdentityBlock = &SDEVICE_IDENTITY_BLOCK(VirtualMemory),
      .LatestStatus  = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
      .Identifier    = identifier
   };

   *instance->Init = *_init;

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   ThisAddressType *addressMap = SDeviceAllocateMemory(sizeof(ThisAddressType[_init->ChunksCount]));

   addressMap[0] = 0;

   for(size_t i = 1; i < _init->ChunksCount; i++)
      SDeviceEvalAssert(TRY_ADD_INT_CHECKED(addressMap[i - 1], _init->Chunks[i].Size, &addressMap[i]),);

   *instance->Runtime = (ThisRuntimeData)
   {
      .AddressMap = addressMap
   };
#else
   ThisAddressType address = _init->Chunks[0].Size - 1;

   for(size_t i = 1; i < _init->ChunksCount; i++)
      SDeviceEvalAssert(TRY_ADD_INT_CHECKED(address, _init->Chunks[i].Size, &address),);

   *instance->Runtime = (ThisRuntimeData)
   {
      .HighestAddress = address
   };
#endif

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer)
{
   SDeviceAssert(handlePointer);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   SDeviceFreeMemory(handle->Runtime->AddressMap);
   handle->Runtime->AddressMap = NULL;
#endif

   SDeviceFreeHandle(handle);

   *_handlePointer = NULL;
}

SDevicePropertyStatus VirtualMemorySDeviceRead(
      ThisHandle               *handle,
      const ThisReadParameters *parameters,
      const void               *context)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   SDeviceAssert(parameters);

   SDeviceAssert(parameters->Data || parameters->Size <= 0);

   return PerformOperation(
         handle, IO_OPERATION(Read), &((const ReadOperationParameters *)parameters)->AsBase, context);
}

SDevicePropertyStatus VirtualMemorySDeviceWrite(
      ThisHandle                *handle,
      const ThisWriteParameters *parameters,
      const void                *context)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   SDeviceAssert(parameters);

   SDeviceAssert(parameters->Data || parameters->Size <= 0);

   return PerformOperation(
         handle, IO_OPERATION(Write), &((const WriteOperationParameters *)parameters)->AsBase, context);
}
