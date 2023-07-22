#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

//  ''ping-pong'' a byte between two processes over a pair of pipes

int main(int argc, char *argv[]) 
{
  // Error judgement : this function receives no argc
  if (argc != 1) 
  {
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  int p[2];   
  int pid;    // process id

  pipe(p);    //create a pipe p

    //fork()一定大于0
  if (fork()==0) //子进程在执行
  {
    pid = getpid();   //return int
    char buf[2];

    if (read(p[0], buf, 1) != 1)           //use read to read pipe
    {
      fprintf(2, "failed to read in child\n");
      exit(1);
    }
 
    close(p[0]);                             //read
    printf("%d: received ping\n", pid);      // 父进程打印ping

    if (write(p[1], buf, 1) != 1) 
    {
      fprintf(2, "failed to write in child\n");
      exit(1);
    }
    close(p[1]);               //关闭读端
    exit(0);              

  }
   else            //fork!=0，父进程在执行
  {
    pid = getpid();
    char info[2] = "A";
    char buf[2];
    buf[1] = 0;
    if (write(p[1], info, 1) != 1)    //write one byte from info to p[1] 
    {
      fprintf(2, "failed to write in parent\n");
      exit(1);
    }

    close(p[1]);              //关闭写端
    wait(0);

    if (read(p[0], buf, 1) != 1) 
    {
      fprintf(2, "failed to read in parent\n");
      exit(1);
    }
    printf("%d: received pong\n", pid);
    close(p[0]);
    exit(0);
  }

}

