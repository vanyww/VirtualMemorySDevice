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

static SDevicePropertyStatus ChunkGetCallback(SDEVICE_HANDLE(VirtualMemory) *handle,
                                               const VirtualMemorySDeviceChunk *chunk,
                                               const VirtualMemorySDeviceChunkGetParameters *parameters,
                                               const void *context)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(parameters->Data, &ChunksBuffers[chunkIndex][parameters->Offset], parameters->Size);
   return SDEVICE_PROPERTY_STATUS_OK;
}

static SDevicePropertyStatus ChunkSetCallback(SDEVICE_HANDLE(VirtualMemory) *handle,
                                                const VirtualMemorySDeviceChunk *chunk,
                                                const VirtualMemorySDeviceChunkSetParameters *parameters,
                                                const void *context)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(&ChunksBuffers[chunkIndex][parameters->Offset], parameters->Data, parameters->Size);
   return SDEVICE_PROPERTY_STATUS_OK;
}

static const VirtualMemorySDeviceChunk Chunks[] =
{
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 0 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 1 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 2 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 3 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 4 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGetCallback,
      .Set = ChunkSetCallback,
      .Context = &(const ChunkContext){ 5 },
      .Size = CHUNK_SIZE
   }
};
