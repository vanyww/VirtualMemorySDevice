#include "unity_fixture.h"

static void RunAllTests(void)
{
   RUN_TEST_GROUP(Initialization);
   RUN_TEST_GROUP(Read);
   RUN_TEST_GROUP(Write);
}

int main (int argc, char const *argv[])
{
   UnityMain (argc, argv, RunAllTests);
   return 0;
}
