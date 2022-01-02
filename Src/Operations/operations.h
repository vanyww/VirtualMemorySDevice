#pragma once

#include "../../Inc/VirtualMemorySDevice/core.h"

typedef struct
{
   const VirtualMemoryChunk *Chunk;
   VirtualMemoryBaseType Offset;
} VirtualMemoryPointer;

typedef union
{
   struct
   {
      const void *Data;
      VirtualMemoryBaseType Size;
      VirtualMemoryBaseType Offset;
   } AsCommon;
   VirtualMemoryChunkReadParameters AsRead;
   VirtualMemoryChunkWriteParameters AsWrite;
} VirtualMemoryChunkParameters;

typedef VirtualMemoryStatus (* VirtualMemoryChunkOperation)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                            const VirtualMemoryChunk *,
                                                            const VirtualMemoryChunkParameters *,
                                                            const void *);

bool TryGetVirtualMemoryPointer(__SDEVICE_HANDLE(VirtualMemory) *, VirtualMemoryBaseType, VirtualMemoryPointer *);

VirtualMemoryStatus ReadVirtualMemoryChunk(__SDEVICE_HANDLE(VirtualMemory) *,
                                           const VirtualMemoryChunk *,
                                           const VirtualMemoryChunkParameters *,
                                           const void *);

VirtualMemoryStatus WriteVirtualMemoryChunk(__SDEVICE_HANDLE(VirtualMemory) *,
                                            const VirtualMemoryChunk *,
                                            const VirtualMemoryChunkParameters *,
                                            const void *);
