#include "map.h"

#include <memory.h>

typedef struct
{
   size_t ChunkIdx;
} ChunkContext;

uint8_t ChunksBuffers[CHUNKS_COUNT][CHUNK_SIZE] = { 0 };

static SDevicePropertyStatus ChunkGet(SDEVICE_HANDLE(VirtualMemory) *handle,
                                      const VirtualMemorySDeviceChunk *chunk,
                                      const VirtualMemorySDeviceChunkGetParameters *parameters,
                                      const void *context)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(parameters->Data, &ChunksBuffers[chunkIndex][parameters->Offset], parameters->Size);
   return SDEVICE_PROPERTY_STATUS_OK;
}

static SDevicePropertyStatus ChunkSet(SDEVICE_HANDLE(VirtualMemory) *handle,
                                      const VirtualMemorySDeviceChunk *chunk,
                                      const VirtualMemorySDeviceChunkSetParameters *parameters,
                                      const void *context)
{
   size_t chunkIndex = ((const ChunkContext *)chunk->Context)->ChunkIdx;
   memcpy(&ChunksBuffers[chunkIndex][parameters->Offset], parameters->Data, parameters->Size);
   return SDEVICE_PROPERTY_STATUS_OK;
}

const VirtualMemorySDeviceChunk Chunks[CHUNKS_COUNT] =
{
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 0 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 1 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 2 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 3 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 4 },
      .Size = CHUNK_SIZE
   },
   {
      .Get = ChunkGet,
      .Set = ChunkSet,
      .Context = &(const ChunkContext){ 5 },
      .Size = CHUNK_SIZE
   }
};
