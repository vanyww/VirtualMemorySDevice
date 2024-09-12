#pragma once

#include "../../../private.h"

typedef union
{
   struct
   {
      const void  *Data;
      ThisSizeType Offset;
      ThisSizeType Size;
   } AsAny;

   ThisChunkGetParameters AsGet;
   ThisChunkSetParameters AsSet;
} ChunkOperationParameters;

#define IO_OPERATION_RETURN_VALUE SDevicePropertyStatus
#define IO_OPERATION_ARGUMENTS(handle_name, chunk_name, parameters_name, context_name) (                               \
   ThisHandle                     *handle_name,                                                                        \
   const ThisChunk                *chunk_name,                                                                         \
   const ChunkOperationParameters *parameters_name,                                                                    \
   const void                     *context_name)
#define IO_OPERATION_POINTER(pointer_name)                                                                             \
   IO_OPERATION_RETURN_VALUE (* pointer_name) IO_OPERATION_ARGUMENTS(,,,)
#define IO_OPERATION(operation_name)                                                                                   \
   VirtualMemorySDeviceInternal##operation_name##Operation
#define IO_OPERATION_DECLARATION(operation_name, handle_name, chunk_name, parameters_name, context_name)               \
   IO_OPERATION_RETURN_VALUE                                                                                           \
   IO_OPERATION(operation_name)                                                                                        \
   IO_OPERATION_ARGUMENTS(handle_name, chunk_name, parameters_name, context_name)
