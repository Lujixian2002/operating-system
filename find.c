#include "kernel/types.h"

#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

#define streq(a, b) (strcmp((a), (b)) == 0)


// from ls.c
// 返回路径字符串中最后一个斜杠之后的部分，即文件名或目录名
char* fmtname(char* path) 
{
  static char buf[DIRSIZ + 1];
  char* p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}


void find(char* start_path, char* expr) 
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(start_path, 0)) < 0) 
  {
    fprintf(2, "find: cannot open %s\n", start_path);
    return;
  }

  if (fstat(fd, &st) < 0) 
  {
    fprintf(2, "find: cannot stat %s\n", start_path);
    close(fd);
    return;
  }

  switch (st.type) 
  {
  case T_FILE:
    if (streq(expr, "*") || streq(fmtname(start_path), expr))
      printf("%s\n", start_path);
    break;

  case T_DIR:
    if (strlen(start_path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }

    strcpy(buf, start_path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      // inum == 0 means invalid directory entry
      if (de.inum == 0)
        continue;

      // add de.name to path
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if (streq(de.name, ".") || streq(de.name, ".."))
        continue;

      find(buf, expr);
    }
    break;
  }

  close(fd);
}

int main(int argc, char* argv[]) 
{
    //此函数接受三个或三个以下参数
  if (argc > 3) 
  {
    fprintf(2, "Usage: %s [starting-point...] [expression]\n", argv[0]);
    exit(1);
  }

  if (argc == 2) 
  {
    find(argv[1], "*");
  }

  find(argv[1], argv[2]);

  exit(0);
}