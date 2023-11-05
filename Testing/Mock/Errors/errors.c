#include "errors.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include "unity.h"
#include <stdlib.h>
#include <string.h>

#define ENUM_TO_STRING(enum) #enum


static bool AreLogsShowing = true;
static bool ProcessAssertFailMustBeCalled;
static SDEVICE_HANDLE(VirtualMemory) *AssertFailhandle;

void ShowLog(bool value)
{
   AreLogsShowing = value;
}

void AssertionMustBeFail(bool value)
{
   ProcessAssertFailMustBeCalled = value;
}

void SetAssertFailHandle(SDEVICE_HANDLE(VirtualMemory) *handle)
{
   AssertFailhandle = handle;
}

void SDeviceProcessAssertFail(char *file, int line)
{
   if(AssertFailhandle != NULL)
         SDeviceFree(AssertFailhandle);

   if(ProcessAssertFailMustBeCalled)
   {
      char str[200];
      sprintf(str, "Test pass, assert called on file (%s) line (%d)", file, line);
      TEST_PASS_MESSAGE(str);
   }
   else
   {
      char str[200];
      sprintf(str, "Test fail, assert called on file (%s) line (%d)", file,  line);
      TEST_FAIL_MESSAGE(str);
   }
}

void SDeviceProcessPanic(const void *_handle) {}

void SDeviceProcessLogStatus(const void *handle, const void *extras, size_t extrasSize)
{
   if(!AreLogsShowing)
      return;

   SDeviceHandleHeader *header = (SDeviceHandleHeader *)(handle);
   VirtualMemorySDeviceStatus status = header->LatestStatus;

   char *pStatus = NULL;
   size_t statusSize = 0;

   switch (status)
   {
      case VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS:
         pStatus = ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS);
         statusSize = sizeof(ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_WRONG_ADDRESS));
         break;

      case VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL:
         pStatus = ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL);
         statusSize = sizeof(ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_READ_FAIL));
         break;

      case VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL:
         pStatus = ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL);
         statusSize = sizeof(ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_CHUNK_WRITE_FAIL));
         break;

      case VIRTUAL_MEMORY_SDEVICE_STATUS_OK:
         pStatus = ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_OK);
         statusSize = sizeof(ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_STATUS_OK));
         break;
   }

   if(extras == NULL)
   {
      TEST_MESSAGE(pStatus);
      return;
   }

   VirtualMemorySDeviceChunkStatus chunkStatus;
   memcpy(&chunkStatus, extras, extrasSize);

   char *pChunkStatus = NULL;
   size_t chunkStatusSize = 0;

   switch (chunkStatus)
   {
      case VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL:
         pChunkStatus = ": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL);
         chunkStatusSize = sizeof(": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_VALIDATION_FAIL));
         break;

      case VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL:
         pChunkStatus = ": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL);
         chunkStatusSize = sizeof(": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_PROCESSING_FAIL));
         break;

      case VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK:
         pChunkStatus = ": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK);
         chunkStatusSize = sizeof(": "ENUM_TO_STRING(VIRTUAL_MEMORY_SDEVICE_CHUNK_STATUS_OK));
         break;
   }

   char str[statusSize + chunkStatusSize - 1];
   memcpy(str, pStatus, statusSize);
   memcpy(&str[statusSize - 1], pChunkStatus, chunkStatusSize);
   TEST_MESSAGE(str);
}
