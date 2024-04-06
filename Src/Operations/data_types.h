#pragma once

#include "../private.h"

typedef struct
{
   const ChunkInternal *Chunk;
   SizeType             Offset;
} MemoryReference;

typedef union __attribute__((transparent_union))
{
   const struct
   {
      const void *Data;
      AddressType Address;
      SizeType    Size;
   } *AsCommon;

   const ReadParametersInternal  *AsRead;
   const WriteParametersInternal *AsWrite;
} OperationParameters;

typedef union
{
   struct
   {
      const void *Data;
      SizeType    Offset;
      SizeType    Size;
   } AsCommon;

   ChunkReadParametersInternal  AsRead;
   ChunkWriteParametersInternal AsWrite;
} ChunkOperationParameters;

typedef ChunkStatusInternal (* ChunkOperation)(ThisHandle                     *handle,
                                               const ChunkInternal            *chunk,
                                               const ChunkOperationParameters *parameters,
                                               const void                     *context);
