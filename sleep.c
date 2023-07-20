#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


//argc 是命令行参数数量
int main(int argc, char *argv[]) 
{

  if (argc != 2) 
  {
    // fprintf(2, "ERROR! Usage:$ sleep <seconds>\nyou should replace <seconds> to an actual number\n");
    fprintf(2, "Usage: sleep seconds...\n");
    exit(1);
  }

  int time = atoi(argv[1]);   //convert string to int
  sleep(time);                //system call 系统调用, in 'user.h'可以查 询
  // printf("OK");
  exit(0);


  // $U/_sleep\  needed to be addded in the 'Makefile'
  
}