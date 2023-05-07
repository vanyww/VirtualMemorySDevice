#pragma once

#include "VirtualMemorySDevice/public.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(VirtualMemory);

SDEVICE_RUNTIME_DATA_DECLARATION(VirtualMemory)
{
   size_t TotalChunksSize;
};

SDEVICE_HANDLE_DECLARATION(VirtualMemory);
SDEVICE_INTERNAL_ALIASES_DECLARATION(VirtualMemory);

typedef VirtualMemorySDeviceChunk Chunk;
typedef VirtualMemorySDeviceChunkStatus ChunkStatus;
typedef VirtualMemorySDeviceChunkReadParameters ChunkReadParameters;
typedef VirtualMemorySDeviceChunkWriteParameters ChunkWriteParameters;
typedef VirtualMemorySDeviceReadParameters ReadParameters;
typedef VirtualMemorySDeviceWriteParameters WriteParameters;
