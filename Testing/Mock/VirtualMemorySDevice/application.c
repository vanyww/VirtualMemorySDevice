#include "application.h"

#include <memory.h>


typedef struct
{
   size_t ChunkIdx;
} ChunkContext;

static const VirtualMemorySDeviceChunk Chunks[];
static uint8_t ChunksBuffers[CHUNKS_COUNT][CHUNK_SIZE] = { 0 };

const VirtualMemorySDeviceChunk* GetCunks(void)
{
   return Chunks;
}

void EraseChunksBuffers(void)
{
   memset(ChunksBuffers, 0, CHUNKS_COUNT*CHUNK_SIZE);
}

void WriteChunkBuffer(size_t chunkIdx, uint8_t *src, size_t size)
{
   memcpy(&ChunksBuffers[chunkIdx][0], src, size);
}

void ReadChunkBuffer(size_t chunkIdx, uint8_t *dst, size_t size)
{
   memcpy(dst, &ChunksBuffers[chunkIdx][0], size);
}

static VirtualMemorySDeviceChunkStatus ChunkReadCallback(SDEVICE_HANDLE(VirtualMemory) *handle,
                                                         const VirtualMemorySDeviceChunk *chunk,
                                                         const VirtualMemorySDeviceChunkReadParameters *parameters)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(parameters->Data, &ChunksBuffers[chunkIndex][parameters->Offset], parameters->Size);
   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

static VirtualMemorySDeviceChunkStatus ChunkWriteCallback(SDEVICE_HANDLE(VirtualMemory) *handle,
                                                          const VirtualMemorySDeviceChunk *chunk,
                                                          const VirtualMemorySDeviceChunkWriteParameters *parameters)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(&ChunksBuffers[chunkIndex][parameters->Offset], parameters->Data, parameters->Size);
   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

static const VirtualMemorySDeviceChunk Chunks[] =
{
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 0 },
      .Size = CHUNK_SIZE
   },
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 1 },
      .Size = CHUNK_SIZE
   },
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 2 },
      .Size = CHUNK_SIZE
   },
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 3 },
      .Size = CHUNK_SIZE
   },
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 4 },
      .Size = CHUNK_SIZE
   },
   {
      .Read = ChunkReadCallback,
      .Write = ChunkWriteCallback,
      .Context = &(const ChunkContext){ 5 },
      .Size = CHUNK_SIZE
   }
};
