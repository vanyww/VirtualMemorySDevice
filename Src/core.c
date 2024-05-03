#include "Operations/operations.h"

#include "SDeviceCore/heap.h"

SDEVICE_IDENTITY_BLOCK_DEFINITION(VirtualMemory,
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

static inline void InitializeThisHandleRuntimeData(ThisHandle *handle)
{
   const ChunkInternal *chunks = handle->Init->Chunks;
   size_t chunksCount = handle->Init->ChunksCount;

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   AddressType highestAddress = 0;
   AddressType *chunkAddressMap = SDeviceMalloc(sizeof(AddressType[chunksCount]));

   for(size_t i = 0; i < chunksCount - 1; i++)
   {
      chunkAddressMap[i] = highestAddress;
      SDeviceEvalAssert(TRY_ADD_INT_CHECKED(highestAddress, chunks[i].Size, &highestAddress),);
   }

   chunkAddressMap[chunksCount - 1] = highestAddress;
   SDeviceEvalAssert(TRY_ADD_INT_CHECKED(highestAddress, chunks[chunksCount - 1].Size - 1, &highestAddress),);

   *handle->Runtime = (ThisRuntimeData)
   {
      .HighestAddress  = highestAddress,
      .ChunkAddressMap = chunkAddressMap
   };
#else
   AddressType highestAddress = chunks[0].Size - 1;

   for(size_t i = 1; i < chunksCount; i++)
      SDeviceEvalAssert(TRY_ADD_INT_CHECKED(highestAddress, chunks[i].Size, &highestAddress),);

   *handle->Runtime = (ThisRuntimeData)
   {
      .HighestAddress = highestAddress
   };
#endif
}

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, owner, identifier, context)
{
   SDeviceAssert(init != NULL);

   const ThisInitData *_init = init;

   SDeviceAssert(_init->Chunks != NULL);
   SDeviceAssert(_init->ChunksCount > 0);

#if SDEVICE_USE_ASSERT
   for(size_t i = 0; i < _init->ChunksCount; i++)
      SDeviceAssert(_init->Chunks[i].Size > 0);
#endif

   ThisHandle *handle = SDeviceAllocHandle(sizeof(ThisInitData), sizeof(ThisRuntimeData));
   handle->Header = (SDeviceHandleHeader)
   {
      .Context       = context,
      .OwnerHandle   = owner,
      .IdentityBlock = &SDEVICE_IDENTITY_BLOCK(VirtualMemory),
      .LatestStatus  = VIRTUAL_MEMORY_SDEVICE_STATUS_OK,
      .Identifier    = identifier
   };
   *handle->Init = *_init;

   InitializeThisHandleRuntimeData(handle);

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, handlePointer)
{
   SDeviceAssert(handlePointer != NULL);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   SDeviceFree(handle->Runtime->ChunkAddressMap);

   handle->Runtime->ChunkAddressMap = NULL;
#endif

   SDeviceFreeHandle(handle);
   *_handlePointer = NULL;
}

SDevicePropertyStatus VirtualMemorySDeviceRead(ThisHandle                   *handle,
                                               const ReadParametersInternal *parameters,
                                               const void                   *context)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   SDeviceAssert(parameters != NULL);

   SDeviceAssert(parameters->Data != NULL || parameters->Size == 0);

   return PerformMemoryOperation(handle, ReadChunk, parameters, context);
}

SDevicePropertyStatus VirtualMemorySDeviceWrite(ThisHandle                    *handle,
                                                const WriteParametersInternal *parameters,
                                                const void                    *context)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   SDeviceAssert(parameters != NULL);

   SDeviceAssert(parameters->Data != NULL || parameters->Size == 0);

   return PerformMemoryOperation(handle, WriteChunk, parameters, context);
}
