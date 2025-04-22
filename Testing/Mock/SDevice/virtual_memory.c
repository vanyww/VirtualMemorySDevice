#include "virtual_memory.h"

#include <memory.h>

SDEVICE_HANDLE(VirtualMemory) * VirtualMemoryMockCreateInstance(void)
{
   SDEVICE_INIT_DATA(VirtualMemory) init =
   {
      .Chunks = Chunks,
      .ChunksCount = CHUNKS_COUNT
   };

   return SDEVICE_CREATE_HANDLE(VirtualMemory)(&init, NULL);
}

void VirtualMemoryMockEraseChunksBuffers(void)
{
   memset(ChunksBuffers, 0, CHUNKS_COUNT*CHUNK_SIZE);
}

void VirtualMemoryMockReadChunkBuffer(size_t chunkIdx, uint8_t *dst, size_t size)
{
   memcpy(dst, &ChunksBuffers[chunkIdx][0], size);
}

void VirtualMemoryMockWriteChunkBuffer(size_t chunkIdx, uint8_t *src, size_t size)
{
   memcpy(&ChunksBuffers[chunkIdx][0], src, size);
}
