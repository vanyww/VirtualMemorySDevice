#include "private.h"
#include "Io/Chunks/get.h"
#include "Io/Chunks/set.h"

#include "SDeviceCore/heap.h"

const IO_OPERATION_POINTER(IoOperations[]) =
{
   [VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_READ]  = IO_OPERATION(Get),
   [VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_WRITE] = IO_OPERATION(Set)
};

static_assert(LENGTHOF(IoOperations) == VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPES_COUNT);

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, context)
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

   instance->Context = context;
   *instance->Init = *_init;

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   ThisAddressType *addressMap = SDeviceAllocateMemory(sizeof(ThisAddressType[_init->ChunksCount]));

   addressMap[0] = 0;

   for(size_t i = 0; i < _init->ChunksCount - 1; i++)
      SDeviceEvalAssert(TRY_ADD_INT_CHECKED(addressMap[i], _init->Chunks[i].Size, &addressMap[i + 1]),);

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

   SDeviceAssert(handle);

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   SDeviceFreeMemory(handle->Runtime->AddressMap);
   handle->Runtime->AddressMap = NULL;
#endif

   SDeviceFreeHandle(handle);

   *_handlePointer = NULL;
}

SDevicePropertyStatus VirtualMemorySDeviceInvokeOperation(
      ThisHandle                                    *handle,
      const VirtualMemorySDeviceOperationParameters *operationParameters,
      const void                                    *callParameters)
{
   SDeviceAssert(handle);

   SDeviceAssert(operationParameters);

   SDeviceAssert(operationParameters->AsAny.Data || operationParameters->AsAny.Size <= 0);

   SDevicePropertyStatus status;
   ThisSizeType size = operationParameters->AsAny.Size;
   IO_OPERATION_POINTER(operation) = IoOperations[operationParameters->Type];

   if(size > 0)
   {
      ThisAddressType lastAddress, firstAddress = operationParameters->AsAny.Address;

      if(TRY_ADD_INT_CHECKED(firstAddress, size - 1, &lastAddress) && lastAddress <= GET_HIGHEST_ADDRESS(handle))
      {
         MemoryReference memoryReference = FindMemoryReference(handle, firstAddress);
         ChunkOperationParameters chunkParameters =
         {
            .AsAny =
            {
               .Data   = operationParameters->AsAny.Data,
               .Offset = memoryReference.Offset,
               .Size   = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size)
            }
         };

         status = operation(handle, memoryReference.Chunk, &chunkParameters, callParameters);

         if(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size)
         {
            chunkParameters.AsAny.Offset = 0;

            do
            {
               memoryReference.Chunk++;
               size -= chunkParameters.AsAny.Size;
               chunkParameters.AsAny.Data += chunkParameters.AsAny.Size;
               chunkParameters.AsAny.Size = MIN(memoryReference.Chunk->Size, size);

               status = operation(handle, memoryReference.Chunk, &chunkParameters, callParameters);
            }
            while(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size);
         }
      }
      else
      {
         status = SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR;
      }
   }
   else
   {
      status = SDEVICE_PROPERTY_STATUS_OK;
   }

   return status;
}
