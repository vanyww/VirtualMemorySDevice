#pragma once

#include "data_types.h"

static MemoryReference FindMemoryReference(ThisHandle *handle, uintptr_t address)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   size_t leftChunkIdx = 0,
          rightChunkIdx = handle->Init->ChunksCount - 1;

   for(;;)
   {
      size_t middleChunkIdx = leftChunkIdx + (rightChunkIdx - leftChunkIdx) / 2;
      AddressType middleChunkAddress = handle->Runtime->ChunkAddressMap[middleChunkIdx];
      const ChunkInternal *middleChunk = &handle->Init->Chunks[middleChunkIdx];

      if(address < middleChunkAddress)
      {
         rightChunkIdx = middleChunkIdx - 1;
      }
      else if(address > middleChunkAddress + (middleChunk->Size - 1))
      {
         leftChunkIdx = middleChunkIdx + 1;
      }
      else
      {
         return (MemoryReference)
         {
            .Chunk  = middleChunk,
            .Offset = address - middleChunkAddress
         };
      }
   }
#else
   const ChunkInternal *chunk = handle->Init->Chunks;
   AddressType lastAddress = chunk->Size - 1;

   while(address > lastAddress)
   {
      chunk++;
      lastAddress += chunk->Size;
   }

   return (MemoryReference)
   {
      .Chunk  = chunk,
      .Offset = chunk->Size - ((lastAddress - address) + 1)
   };
#endif
}

__attribute__((unused))
static AddressType FindChunkAddress(ThisHandle *handle, const ChunkInternal *chunk)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   return handle->Runtime->ChunkAddressMap[chunk - handle->Init->Chunks];
#else
   AddressType chunkAddress = 0;

   while(chunk > handle->Init->Chunks)
   {
      chunk--;
      chunkAddress += chunk->Size;
   }

   return chunkAddress;
#endif
}
