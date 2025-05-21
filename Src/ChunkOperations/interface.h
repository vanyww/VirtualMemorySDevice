#pragma once

#include "../private.h"

typedef union
{
   VirtualMemorySDeviceChunkGetParameters AsGet;
   VirtualMemorySDeviceChunkSetParameters AsSet;

   struct
   {
      const void *Data;
      size_t      Offset;
      size_t      Size;
   } AsAny;
} ChunkOperationParameters;

#define CHUNK_OPERATION_RETURN_VALUE SDevicePropertyStatus
#define CHUNK_OPERATION_ARGUMENTS(this_name, chunk_name, parameters_name, call_parameters_name) (                      \
   ThisHandle                      *this_name,                                                                         \
   const VirtualMemorySDeviceChunk *chunk_name,                                                                        \
   const ChunkOperationParameters  *parameters_name,                                                                   \
   const void                      *call_parameters_name)
#define CHUNK_OPERATION_POINTER(pointer_name)                                                                          \
   CHUNK_OPERATION_RETURN_VALUE (* pointer_name) CHUNK_OPERATION_ARGUMENTS(,,,)
#define CHUNK_OPERATION(name)                                                                                          \
   VirtualMemorySDeviceInternal##name##Operation
#define CHUNK_OPERATION_DECLARATION(name, this_name, chunk_name, parameters_name, call_parameters_name)                \
   CHUNK_OPERATION_RETURN_VALUE                                                                                        \
   CHUNK_OPERATION(name)                                                                                               \
   CHUNK_OPERATION_ARGUMENTS(this_name, chunk_name, parameters_name, call_parameters_name)
