#pragma once

#include "VirtualMemorySDevice/public.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(VirtualMemory);

SDEVICE_INTERNAL_ALIASES_DECLARATION(VirtualMemory);

SDEVICE_RUNTIME_DATA_DECLARATION(VirtualMemory)
{
#if VIRTUAL_MEMORY_SDEVICE_USE_BINARY_SEARCH
   uintptr_t *AddressTable;
#else
   uintptr_t  HighestAddress;
#endif
};

SDEVICE_HANDLE_DECLARATION(VirtualMemory);
