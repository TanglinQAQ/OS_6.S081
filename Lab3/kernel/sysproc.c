#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
/*int pgaccess(void *base, int len, void *mask);
base代表检测的页表起始虚拟地址
len代表检测的页表个数
mask为掩码的地址
*/
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va, target_addr;
  int len;
  uint32 res = 0;//结果
  //读入va和len
  if (argaddr(0, &va) < 0)
    return -1;
  if (argint(1, &len) < 0)
    return -1;
  if (argaddr(2, &target_addr) < 0)
    return -1;
  //判断读入值是否合法
  if (len < 0 || len>32)
    return -1;
  struct proc* p = myproc();
  int i;
  for (i = 0;i < len;i++) {
    if (va > MAXVA)
      return -1;
    pte_t* pte = walk(p->pagetable, va, 0);
    if (pte == 0)
      return -1;
    if (*pte & PTE_A) {
      res |= (1 << i);//结果对应的那一位置为1
      *pte &= (~PTE_A);//把PTE_A置为0
    }
    va += PGSIZE;
  }
  if (copyout(p->pagetable, target_addr, (char*)&res, sizeof(res)) < 0)
    return -1;
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
