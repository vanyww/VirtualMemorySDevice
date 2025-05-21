#include "private.h"
#include "Maps/chunk_operation_map.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/assert.h"

typedef struct
{
   const VirtualMemorySDeviceChunk *Chunk;
   size_t                           Offset;
} MemoryReference;

__attribute__((always_inline))
static inline uintptr_t GetHighestAddress(ThisHandle *this)
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
{
   size_t lastIdx = this->Init->ChunksCount - 1;

   return this->Runtime->AddressTable[lastIdx] + (this->Init->Chunks[lastIdx].Size - 1);
}
#else
{
   return this->Runtime->HighestAddress;
}
#endif

static inline MemoryReference FindMemoryReference(ThisHandle *handle, uintptr_t address)
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
{
   size_t leftIdx = 0, rightIdx = handle->Init->ChunksCount - 1;

   for(;;)
   {
      size_t idx = leftIdx + (rightIdx - leftIdx) / 2;
      uintptr_t chunkAddress = handle->Runtime->AddressTable[idx];
      const VirtualMemorySDeviceChunk *chunk = &handle->Init->Chunks[idx];

      if(address < chunkAddress)
      {
         rightIdx = idx - 1;
      }
      else if(address > chunkAddress + (chunk->Size - 1))
      {
         leftIdx = idx + 1;
      }
      else
      {
         return (MemoryReference)
         {
            .Chunk  = chunk,
            .Offset = address - chunkAddress
         };
      }
   }
}
#else
{
   const VirtualMemorySDeviceChunk *chunk = handle->Init->Chunks;
   uintptr_t lastAddress = chunk->Size - 1;

   while(address > lastAddress)
      lastAddress += (++chunk)->Size;

   return (MemoryReference)
   {
      .Chunk  = chunk,
      .Offset = chunk->Size - ((lastAddress - address) + 1)
   };
}
#endif

SDEVICE_CREATE_HANDLE_DECLARATION(VirtualMemory, init, context)
{
   const ThisInitData *_init = init;

   SDeviceAssert(_init);
   SDeviceAssert(_init->Chunks);
   SDeviceAssert(_init->ChunksCount > 0);

#if SDEVICE_USE_ASSERT
   for(size_t i = 0; i < _init->ChunksCount; i++)
      SDeviceAssert(_init->Chunks[i].Size > 0);
#endif

   ThisHandle *instance =
         SDeviceAllocateHandle(
               sizeof(*instance->Init),
               sizeof(*instance->Runtime));

   *instance->Init = *_init;

   instance->Context = context;

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   uintptr_t *addressTable =
         SDeviceAllocateMemory(
               sizeof(uintptr_t[_init->ChunksCount]));

   addressTable[0] = 0;

   for(size_t i = 0; i < _init->ChunksCount - 1; i++)
   {
      SDeviceEvalAssert(
            TRY_ADD_INT_CHECKED(
                  addressTable[i],
                  _init->Chunks[i].Size,
                  &addressTable[i + 1]),);
   }

   instance->Runtime->AddressTable = addressTable;
#else
   uintptr_t address = _init->Chunks[0].Size - 1;

   for(size_t i = 1; i < _init->ChunksCount; i++)
   {
      SDeviceEvalAssert(
            TRY_ADD_INT_CHECKED(
                  address,
                  _init->Chunks[i].Size,
                  &address),);
   }

   instance->Runtime->HighestAddress = address;
#endif

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(VirtualMemory, this)
{
   ThisHandle *_this = this;

   SDeviceAssert(_this);

#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   SDeviceFreeMemory(_this->Runtime->AddressTable);
#endif

   SDeviceFreeHandle(_this);
}

SDevicePropertyStatus VirtualMemorySDeviceAccess(
      ThisHandle                                    *this,
      const VirtualMemorySDeviceOperationParameters *operationParameters,
      const void                                    *callParameters)
{
   SDeviceAssert(this);
   SDeviceAssert(operationParameters);
   SDeviceAssert(
         operationParameters->Interface.AsAny.Data ||
         operationParameters->Interface.AsAny.Size <= 0);

   size_t size = operationParameters->Interface.AsAny.Size;

   if(size <= 0)
      return SDEVICE_PROPERTY_STATUS_OK;

   uintptr_t lastAddress, firstAddress = operationParameters->Interface.AsAny.Address;

   if(!TRY_ADD_INT_CHECKED(firstAddress, size - 1, &lastAddress) || lastAddress > GetHighestAddress(this))
      return SDEVICE_PROPERTY_STATUS_VALIDATION_ERROR;

   MemoryReference memoryReference = FindMemoryReference(this, firstAddress);
   ChunkOperationParameters chunkParameters =
   {
      .AsAny =
      {
         .Data   = operationParameters->Interface.AsAny.Data,
         .Offset = memoryReference.Offset,
         .Size   = MIN(memoryReference.Chunk->Size - memoryReference.Offset, size)
      }
   };

   CHUNK_OPERATION_POINTER(operation) = ChunkOperationMap[operationParameters->TypeIdx];
   SDevicePropertyStatus status =
         operation(this, memoryReference.Chunk, &chunkParameters, callParameters);

   if(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size)
   {
      chunkParameters.AsAny.Offset = 0;

      do
      {
         memoryReference.Chunk++;
         size -= chunkParameters.AsAny.Size;
         chunkParameters.AsAny.Data += chunkParameters.AsAny.Size;
         chunkParameters.AsAny.Size = MIN(memoryReference.Chunk->Size, size);

         status = operation(this, memoryReference.Chunk, &chunkParameters, callParameters);
      }
      while(status == SDEVICE_PROPERTY_STATUS_OK && size > chunkParameters.AsAny.Size);
   }

   return status;
}
