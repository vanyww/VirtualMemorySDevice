#pragma once

#include "../../Inc/VirtualMemorySDevice/core.h"

typedef struct
{
   const VirtualMemorySDeviceChunk *Chunk;
   VirtualMemorySDeviceBaseType Offset;
} VirtualMemoryPointer;

bool VirtualMemoryTryFindChunk(__SDEVICE_HANDLE(VirtualMemory) *, VirtualMemorySDeviceBaseType, VirtualMemoryPointer *);
