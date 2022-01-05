#pragma once

#include "VirtualMemorySDevice/core.h"

#define __MOCK_CHUNK_SIZE 2U
#define __MOCK_CHUNKS_COUNT 3U

extern uint8_t MockChunksBuffers[__MOCK_CHUNKS_COUNT][__MOCK_CHUNK_SIZE];
extern const VirtualMemoryChunk MockChunks[];
extern const size_t MockChunksCount;
extern const __SDEVICE_CONSTANT_DATA(VirtualMemory) ConstandData;
