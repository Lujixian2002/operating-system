struct stat;
struct rtcdate;

// system calls
int fork(void);                               // create a process, return child's PID
int exit(int) __attribute__((noreturn));      // terminate the current process;status reported to wait(),No return
int wait(int*);                               // wait for a child to exit;exit status in *
int pipe(int*);                               // create a pip, put read/write file descriptors in p[0] and p[1]
int write(int, const void*, int);             // Write n bytes from buf to file descriptor fd; returns n.
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);                      //
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);                        //休眠
int uptime(void);                         





// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
