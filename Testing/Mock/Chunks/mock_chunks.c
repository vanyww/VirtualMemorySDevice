#include "mock_chunks.h"

#include "SDeviceCore/common.h"

#include <memory.h>

typedef struct { size_t ChunkIndex; } MockChunkContext;
uint8_t MockChunksBuffers[__MOCK_CHUNKS_COUNT][__MOCK_CHUNK_SIZE] = { 0 };

static VirtualMemorySDeviceChunkStatus MockChunkRead(SDEVICE_HANDLE(VirtualMemory) *handle,
                                                     const VirtualMemorySDeviceChunk *chunk,
                                                     const VirtualMemorySDeviceChunkReadParameters *parameters)
{
   size_t chunkIndex = ((const MockChunkContext *)chunk->Context)->ChunkIndex;
   memcpy(parameters->Data, &MockChunksBuffers[chunkIndex][parameters->Offset], parameters->Size);
   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

static VirtualMemorySDeviceChunkStatus MockChunkWrite(SDEVICE_HANDLE(VirtualMemory) *handle,
                                                      const VirtualMemorySDeviceChunk *chunk,
                                                      const VirtualMemorySDeviceChunkWriteParameters *parameters)
{
   size_t chunkIndex = ((const MockChunkContext *)chunk->Context)->ChunkIndex;
   memcpy(&MockChunksBuffers[chunkIndex][parameters->Offset], parameters->Data, parameters->Size);
   return VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK;
}

const VirtualMemorySDeviceChunk MockChunks[] =
{
   {
      .Read = MockChunkRead,
      .Write = MockChunkWrite,
      .Context = &(const MockChunkContext){ 0 },
      .Size = __MOCK_CHUNK_SIZE
   },
   {
      .Read = MockChunkRead,
      .Write = MockChunkWrite,
      .Context = &(const MockChunkContext){ 1 },
      .Size = __MOCK_CHUNK_SIZE
   },
   {
      .Read = NULL,
      .Write = NULL,
      .Context = &(const MockChunkContext){ 2 },
      .Size = __MOCK_CHUNK_SIZE
   },
};

const size_t MockChunksCount = LENGTHOF(MockChunks);
