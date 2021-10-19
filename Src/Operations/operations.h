#pragma once

#include "../../Inc/VirtualMemorySDevice/core.h"

VirtualMemorySDeviceStatus VirtualMemoryTryReadChunk(__SDEVICE_HANDLE(VirtualMemory) *,
                                                     void *,
                                                     const VirtualMemorySDeviceChunk *,
                                                     VirtualMemorySDeviceFunctionParameters *);

VirtualMemorySDeviceStatus VirtualMemoryTryWriteChunk(__SDEVICE_HANDLE(VirtualMemory) *,
                                                      void *,
                                                      const VirtualMemorySDeviceChunk *,
                                                      VirtualMemorySDeviceFunctionParameters *);
