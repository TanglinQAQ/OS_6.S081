#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WRITE 1
#define READ 0

void calc_prime(int* num, int count)
{
  int pip[2], prime = 0;
  pipe(pip);
  if (count == 0)
    return;
  else {
    prime = num[0];
    printf("prime %d\n", prime);//打印第一个数，为素数
  }
  if (fork() == 0) {
    //子进程：管道第一个数即为素数
    close(pip[READ]);
    int i;
    for (i = 1;i < count;i++) {
      //printf("%d", *(num + i));
      write(pip[WRITE], (char*)(num + i), 4);
    }
    close(pip[WRITE]);
    exit(0);
  }
  else {
    close(pip[WRITE]);
    //父进程：取出num的倍数
    count = 0;
    int t;
    while (read(pip[READ], &t, 4)) {
      //printf("t:%d", t);
      if (t % prime != 0) {
        num[count] = t;
        count++;
      }
    }
    calc_prime(num, count);
    close(pip[READ]);
    wait(0);
  }
}

int main()
{
  int num[34];
  int i;
  for (i = 0;i < 34;i++)
    num[i] = i + 2;
  calc_prime(num, 34);
  exit(0);
}