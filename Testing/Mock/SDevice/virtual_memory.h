#pragma once

#include "Bindings/Memory/map.h"

#define _cleanup __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory))))

SDEVICE_HANDLE(VirtualMemory) * VirtualMemoryMockCreateInstance(void);

void VirtualMemoryMockEraseChunksBuffers(void);
void VirtualMemoryMockReadChunkBuffer(size_t chunkIdx, uint8_t *dst, size_t size);
void VirtualMemoryMockWriteChunkBuffer(size_t chunkIdx, uint8_t *src, size_t size);
