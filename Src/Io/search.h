#pragma once

#include "../private.h"

typedef struct
{
   const ThisChunk *Chunk;
   ThisSizeType     Offset;
} MemoryReference;

static MemoryReference FindMemoryReference(ThisHandle *handle, uintptr_t address)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   size_t leftIdx  = 0,
          rightIdx = handle->Init->ChunksCount - 1;

   for(;;)
   {
      size_t           idx          = leftIdx + (rightIdx - leftIdx) / 2;
      const ThisChunk *chunk        = &handle->Init->Chunks[idx];
      ThisAddressType  chunkAddress = handle->Runtime->AddressMap[idx];

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
#else
   const ThisChunk *chunk            = handle->Init->Chunks;
   ThisAddressType  chunkLastAddress = chunk->Size - 1;

   while(address > chunkLastAddress)
      chunkLastAddress += (++chunk)->Size;

   return (MemoryReference)
   {
      .Chunk  = chunk,
      .Offset = chunk->Size - ((chunkLastAddress - address) + 1)
   };
#endif
}

__attribute__((unused))
static ThisAddressType FindChunkAddress(ThisHandle *handle, const ThisChunk *chunk)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   return handle->Runtime->AddressMap[chunk - handle->Init->Chunks];
#else
   ThisAddressType address = 0;

   while(chunk > handle->Init->Chunks)
      address += (--chunk)->Size;

   return address;
#endif
}
