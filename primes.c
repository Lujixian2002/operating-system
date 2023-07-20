// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fs.h"
// #include "kernel/fcntl.h"

// #define SIZE 35

// void prime(int read_fd) 
// {
//     int read_buf[SIZE] = {0};
//     int count = read(read_fd, read_buf, sizeof(int) * SIZE);
//     count /= sizeof(int); // convert byte count to int count

//     if (count <= 0)
//     {
//         exit(0); // If no data was read, then it's time to exit.
//     }

//     int first =read_buf[0];
//     printf("prime %d\n", first);

//     int write_buf[SIZE] = {0};
//     int i = 0;
    
//     for(int j=1;j<count;j++)
//     {
//         如果后续是first的倍数，则跳过
//         if (read_buf[j] % first != 0)
//         {
//             否则记录
//             write_buf[i++] = read_buf[j];
//         }
//     }
//     if (i == 0)
//     {
//         exit(0); // If no numbers written, then it's time to exit.
//     }

//     int p[2];
//     int ret = pipe(p);
//     if (ret != 0) {
//         fprintf(2, "Error: failed to create pipe. Error code: %d\n", ret);
//         exit(1);
//     }

// if (fork() == 0) 
// {
//     close(p[1]); // Close write end in the child process
//     prime(p[0]); // Call prime function in child process
// }
// else
// {
//     close(p[0]); // Close read end in the parent process
//     for (int k = 0; k < i; k++)
//     {
//         write(p[1], &write_buf[k], sizeof(int)); // write to the pipe in the parent process
//     }
//     close(p[1]); // Close write end in the parent process
//     wait((int *) 0); // Wait for child process to finish
//     exit(0);
// }

// }


// int main(int argc, char *argv[])
// {
//     只接受一个参数
//     if (argc != 1) 
//     {
//         fprintf(2, "Usage: primes\n");   //error input
//         exit(1);
//     }

//     int p[2];
//     int ret = pipe(p);
//     错误判断
//     if (ret != 0) {
//         fprintf(2, "Error: failed to create pipe. Error code: %d\n", ret);
//         exit(1);
// }

//     int pid =fork();

//     错误判断
//     if(pid<0) 
//     {
//         fprintf(2,"fork ERROR ...");
//         exit(1);
//     }
//  else if(pid>0)    //father
// {
//     关闭读通道
//     close(p[0]);
//     向管道内写入2-35的整数
//     for(int i=2;i<=SIZE;i++)
//     {
//         write(p[1],&i,sizeof(int));
//     }
//     关闭写通道
//     close(p[1]);
//     等待子进程结束
//     wait(0);   
//     exit(0);
// }
// else           //child  fork()==0 子进程在执行
// {
//     close(p[1]); // Close write end in the child process
//     开始执行prime计算
//     prime(p[0]);
//     exit(0);
// }
// exit(0);
// }

#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

void sieveProc(int left[2]) {
  close(left[WRITE]);

  int prime, temp, pid, right[2];

  if (read(left[READ], &prime, sizeof(int)) == 0) {
    close(left[READ]);
    exit(0);
  }

  printf("prime %d\n", prime);

  pipe(right);
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork error...\n");
    close(right[READ]);
    close(right[WRITE]);
    close(left[READ]);
    exit(1);
  } else if (pid > 0) {
    close(right[READ]);
    while (read(left[READ], &temp, sizeof(int))) {
      if (temp % prime == 0)
        continue;
      write(right[WRITE], &temp, sizeof(int));
    }
    close(right[WRITE]);
    wait(0);
    exit(0);
  } else {
    sieveProc(right);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(stderr, "Usage: primes\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork error...\n");
    exit(1);
  } else if (pid > 0) {
    close(p[READ]);

    for (int i = 2; i <= 35; i++) {
      write(p[WRITE], &i, sizeof(int));
    }

    close(p[WRITE]);
    wait(0);
    exit(0);
  } else {
    sieveProc(p);
    exit(0);
  }
}