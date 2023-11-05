#pragma once

#include "VirtualMemorySDevice/public.h"

#include <stdbool.h>


void ShowLog(bool value);
void AssertionMustBeFail(bool value);
void SetAssertFailHandle(SDEVICE_HANDLE(VirtualMemory) *handle);
