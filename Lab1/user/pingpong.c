#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
int main(int argc, char* argv[]) {
  int p[2], c[2];
  pipe(p);
  pipe(c);
  
  if (fork() == 0) {//子进程的fork()==0
    char c_s[10];
    close(c[READ]);
    close(p[WRITE]);
    read(p[READ], c_s, 4);
    printf("%d: received %s\n", getpid(), c_s);
    write(c[WRITE], "pong", 4);
    close(c[WRITE]);
  }
  else {//父进程fork()！=0
    char p_s[10];
    close(p[READ]);
    close(c[WRITE]);
    write(p[WRITE], "ping", 4);
    close(p[WRITE]);
    read(c[READ], p_s, 4);
    printf("%d: received %s\n", getpid(),p_s);
  }
  exit(0);
}
