#pragma once

#include "../private.h"

typedef struct
{
   const ChunkInternal *Chunk;
   size_t               Offset;
} MemoryReference;

typedef union __attribute__((transparent_union))
{
   const struct
   {
      const void *Data;
      const void *CallContext;
      uintptr_t   Address;
      size_t      Size;
   } *AsCommon;

   const ReadParametersInternal  *AsRead;
   const WriteParametersInternal *AsWrite;
} OperationParameters;

typedef union
{
   struct
   {
      const void *Data;
      const void *CallContext;
      size_t      Offset;
      size_t      Size;
   } AsCommon;

   ChunkReadParametersInternal  AsRead;
   ChunkWriteParametersInternal AsWrite;
} ChunkOperationParameters;

typedef ChunkStatusInternal (* ChunkOperation)(ThisHandle                     *handle,
                                               const ChunkInternal            *chunk,
                                               const ChunkOperationParameters *parameters);
