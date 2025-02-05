#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

// * labs4 - #2
uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  backtrace();
  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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

// * labs4 - #3
uint64
sys_sigalarm(void)
{
  int ticks;
  uint64 handler;

  // get args from user - argraw checks for case
  argint(0, &ticks);
  argaddr(1,&handler);
  
  // handle disabling the alarm
  if(ticks == 0 && handler == 0)
  {
      myproc()->alarm_req=0;
      myproc()->alarm_active=0;
      myproc()->alarm_ticks=0;
      myproc()->ticks_cnt=0;
      myproc()->alarm_handler=0;
      return 0;
  }

  // call alarm
  sigalarm(ticks, handler);
  return 0;
}

// * labs4 - #3
uint64
sys_sigreturn(void)
{
  sigreturn();
  return 0;
}


