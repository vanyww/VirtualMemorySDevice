#pragma once

#include "VirtualMemorySDevice/public.h"

#define CHUNK_SIZE 10u
#define CHUNKS_COUNT 6u

extern uint8_t ChunksBuffers[CHUNKS_COUNT][CHUNK_SIZE];
extern const VirtualMemorySDeviceChunk Chunks[CHUNKS_COUNT];
