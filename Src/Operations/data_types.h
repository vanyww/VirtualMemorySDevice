#include "../private.h"

typedef struct
{
   const Chunk *Chunk;
   intptr_t     Offset;
} VirtualMemoryReference;

typedef union __attribute__((transparent_union))
{
   const struct
   {
      uintptr_t   Address;
      size_t      Size;
      uintptr_t   DataPointer;
      const void *CallContext;
   } *AsCommon;
   const ReadParameters  *AsRead;
   const WriteParameters *AsWrite;
} OperationParameters;

typedef union
{
   struct
   {
      uintptr_t   DataPointer;
      intptr_t    Offset;
      size_t      Size;
      const void *CallContext;
   } AsCommon;
   ChunkReadParameters  AsRead;
   ChunkWriteParameters AsWrite;
} ChunkOperationParameters;

typedef ChunkStatus (* ChunkOperation)(ThisHandle                     *handle,
                                       const Chunk                    *chunk,
                                       const ChunkOperationParameters *parameters);
