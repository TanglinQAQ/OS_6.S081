#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX 1024
int main(int argc, char** argv)
{
  char line[MAX];
  char* cmd = argv[1];//存命令
  char* params[MAXARG];//存参数

  int len = 0;
  int i, count = 0;
  for (i = 1;i < argc;i++) {
    params[count++] = argv[i];
  }
  //循环读取标准输入
  while ((len = read(0, line, MAX)) > 0) {
    if (fork() == 0) {
      //子进程：执行命令
      int p = 0;
      for (i = 0;i < len;i++) {
        if (line[i] == ' ' || line[i] == '\n') {
          //一段命令结束
          params[count][p] = 0;
          count++;
          p = 0;
        }
        else {
          //指针！需要申请内存空间
          if (p == 0)
            params[count] = (char*)malloc(MAX);
          params[count][p] = line[i];
          p++;
        }
      }
      exec(cmd, params);
    }
    else
      wait(0);
  }
  exit(0);
}