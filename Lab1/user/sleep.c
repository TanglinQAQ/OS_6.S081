#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char* argv[])
{
  if (argc != 2)
    exit(0);
  printf("test sleep\n");
  sleep(atoi(argv[1]));
  exit(0);
}