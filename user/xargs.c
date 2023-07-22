#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) 
{
    // 不接受一个参数
  if (argc == 1)
  {
    fprintf(2, "Usage: %s [command [initial-arguments]]\n", argv[0]);
    exit(1);
  }

  if (argc - 1 >= MAXARG) 
  {
    fprintf(2, "xargs : maxium amount of args is %d !\n", MAXARG);
    exit(1);
  }

  char buf[1024], *args[MAXARG];
  int num_args;
  int n;
  for (int i = 1; i < argc; i++) 
  {
    args[i - 1] = argv[i];
  }

  num_args = argc - 1;
  char* p = buf;
  while ((n = read(0, p, 1)) > 0) 
  {
    if (*p == '\n') {
      *p = 0;
      if (fork() == 0) {
        args[num_args] = buf;
        exec(args[0], args);
        exit(0);
      }
      else 
      {
        wait(0);
      }
      p = buf;
    }
    else 
    {
      ++p;
    }
  }
  exit(0);
}