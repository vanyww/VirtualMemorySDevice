#pragma once

#include "../ChunkOperations/Get/get.h"
#include "../ChunkOperations/Set/set.h"

#include "SDeviceCore/common.h"

static const CHUNK_OPERATION_POINTER(ChunkOperationMap[]) =
{
   [VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_READ]  = CHUNK_OPERATION(Get),
   [VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDX_WRITE] = CHUNK_OPERATION(Set)
};

static_assert(LENGTHOF(ChunkOperationMap) == VIRTUAL_MEMORY_SDEVICE_OPERATION_TYPE_IDXS_COUNT);
