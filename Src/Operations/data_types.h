#include "../private.h"

typedef struct
{
   const Chunk *Chunk;
   intptr_t Offset;
} VirtualMemoryPointer;

typedef union __attribute__((transparent_union))
{
   const struct
   {
      uintptr_t Address;
      size_t Size;
      uintptr_t DataPointer;
      const void *CallContext;
   } *AsCommon;
   const ReadParameters *AsRead;
   const WriteParameters *AsWrite;
} OperationParameters;

typedef union
{
   struct
   {
      uintptr_t DataPointer;
      size_t Size;
      intptr_t Offset;
      const void *ChunkContext;
      const void *CallContext;
   } AsCommon;
   ChunkReadParameters AsRead;
   ChunkWriteParameters AsWrite;
} ChunkOperationParameters;

typedef ChunkStatus (* ChunkOperation)(ThisHandle *handle,
                                       const Chunk *chunk,
                                       const ChunkOperationParameters *parameters);