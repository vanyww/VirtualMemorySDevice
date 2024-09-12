#pragma once

#include "VirtualMemorySDevice/public.h"

#define CHUNK_SIZE 10U
#define CHUNKS_COUNT 6U
#define VIRTUAL_MEMORY_DISPOSE_HANDLE_CLEANUP_ATTRIBUTE __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(VirtualMemory))))

const VirtualMemorySDeviceChunk* GetCunks(void);

void EraseChunksBuffers(void);
void ReadChunkBuffer(size_t chunkIdx, uint8_t *dst, size_t size);
void WriteChunkBuffer(size_t chunkIdx, uint8_t *src, size_t size);
