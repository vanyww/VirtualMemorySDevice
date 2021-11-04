#include "mock_chunks.h"

#include <memory.h>

typedef struct { size_t ChunkIndex; } MockChunkContext;

uint8_t MockChunksBuffers[__MOCK_CHUNKS_COUNT][__MOCK_CHUNK_SIZE];

static VirtualMemorySDeviceChunkFunctionStatus MockChunkRead(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                             const VirtualMemorySDeviceFunctionParameters *parameters,
                                                             void *data,
                                                             const void *context)
{
   size_t chunkIndex = ((MockChunkContext *)context)->ChunkIndex;
   memcpy(data, &MockChunksBuffers[chunkIndex][parameters->Offset], parameters->BytesCount);
   return VIRTUAL_MEMORY_SDEVICE_STATUS_OK;
}

static VirtualMemorySDeviceChunkFunctionStatus MockChunkWrite(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                              const VirtualMemorySDeviceFunctionParameters *parameters,
                                                              const void *data,
                                                              const void *context)
{
   size_t chunkIndex = ((MockChunkContext *)context)->ChunkIndex;
   memcpy(&MockChunksBuffers[chunkIndex][parameters->Offset], data, parameters->BytesCount);
   return VIRTUAL_MEMORY_SDEVICE_STATUS_OK;
}

VirtualMemorySDeviceChunk MockChunks[] =
{
   {
      .ReadFunction = MockChunkRead,
      .WriteFunction = MockChunkWrite,
      .Context = &(MockChunkContext){ 0 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
   {
      .ReadFunction = MockChunkRead,
      .WriteFunction = MockChunkWrite,
      .Context = &(MockChunkContext){ 1 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
   {
      .ReadFunction = NULL,
      .WriteFunction = NULL,
      .Context = &(MockChunkContext){ 3 },
      .BytesCount = __MOCK_CHUNK_SIZE
   },
};

const size_t MockChunksCount = sizeof(MockChunks) / sizeof(VirtualMemorySDeviceChunk);

const __SDEVICE_CONSTANT_DATA(VirtualMemory) ConstandData =
{
   .Chunks = MockChunks,
   .ChunksCount = MockChunksCount,
   .AddressingStart = 0x00,
   .Context = NULL
};
