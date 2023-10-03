#include "Tests/Initialization/test_initialization.h"
#include "Tests/Operations/Read/test_read.h"
#include "Tests/Operations/Write/test_write.h"

#include <stdio.h>
#include <string.h>

struct
{
   bool (*Function)(void);
   char *Description;
} TestFunctions[] =
{
   { TestVirtualMemoryHandleInitialization, "handle initialization" },

   { TestReadNormal, "read normal" },
   { TestReadEmpty, "read trough empty" },

   { TestWriteNormal, "write normal" },
   { TestWriteEmpty, "write trough empty" }
};

const size_t TestFunctionsCount = sizeof(TestFunctions) / sizeof(*TestFunctions);

int main(void)
{
   int maxMessageLength = 0;
   for(size_t i = 0; i < TestFunctionsCount; i++)
   {
      int length = strlen(TestFunctions[i].Description);
      if(length > maxMessageLength)
         maxMessageLength = length;
   }

   printf("start tests...\n\n");

   for(size_t i = 0; i < TestFunctionsCount; i++)
      printf("%*s - %s\n",
             -maxMessageLength, TestFunctions[i].Description, TestFunctions[i].Function() == true ? "pass" : "fail");

   printf("\nstop tests...\n");
}
