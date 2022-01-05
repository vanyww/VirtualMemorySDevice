#include "mock_chunks.h"

#include <memory.h>

typedef struct { size_t ChunkIndex; } MockChunkContext;

uint8_t MockChunksBuffers[__MOCK_CHUNKS_COUNT][__MOCK_CHUNK_SIZE];

static VirtualMemoryChunkStatus MockChunkRead(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                              const VirtualMemoryChunkReadParameters *parameters,
                                              const void *chunkContext,
                                              const void *callContext)
{
   size_t chunkIndex = ((MockChunkContext *)chunkContext)->ChunkIndex;
   memcpy(parameters->Data, &MockChunksBuffers[chunkIndex][parameters->Offset], parameters->Size);
   return VIRTUAL_MEMORY_CHUNK_STATUS_OK;
}

static VirtualMemoryChunkStatus MockChunkWrite(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                               const VirtualMemoryChunkWriteParameters *parameters,
                                               const void *chunkContext,
                                               const void *callContext)
{
   size_t chunkIndex = ((MockChunkContext *)chunkContext)->ChunkIndex;
   memcpy(&MockChunksBuffers[chunkIndex][parameters->Offset], parameters->Data, parameters->Size);
   return VIRTUAL_MEMORY_CHUNK_STATUS_OK;
}

const VirtualMemoryChunk MockChunks[] =
{
   {
      .Read = MockChunkRead,
      .Write = MockChunkWrite,
      .Context = &(MockChunkContext){ 0 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
   {
      .Read = MockChunkRead,
      .Write = MockChunkWrite,
      .Context = &(MockChunkContext){ 1 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
   {
      .Read = NULL,
      .Write = NULL,
      .Context = &(MockChunkContext){ 2 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
};

const size_t MockChunksCount = sizeof(MockChunks) / sizeof(VirtualMemoryChunk);

const __SDEVICE_CONSTANT_DATA(VirtualMemory) ConstandData =
{
   .ChunksList = &(const VirtualMemoryChunkList)
   {
      .Chunks = MockChunks,
      .Count = sizeof(MockChunks) / sizeof(VirtualMemoryChunk)
   },
   .AddressingStart = 0x00
};
