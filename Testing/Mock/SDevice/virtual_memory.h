#pragma once

#include "Bindings/Memory/map.h"

void VirtualMemoryMockEraseChunksBuffers(void);
void VirtualMemoryMockReadChunkBuffer(size_t chunkIdx, uint8_t *dst, size_t size);
void VirtualMemoryMockWriteChunkBuffer(size_t chunkIdx, uint8_t *src, size_t size);
