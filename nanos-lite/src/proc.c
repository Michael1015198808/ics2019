#include "proc.h"

void naive_uload(PCB *, const char *);
#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
  //context_kload(&pcb[1],(void*)hello_fun);
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/pal");
  context_uload(&pcb[2], "/bin/dummy");
  context_uload(&pcb[3], "/bin/init");
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
    current->cp=prev;
    static uint8_t cnt=0;
    current = (++cnt==0 ? &pcb[0] : &pcb[1]);
    return current->cp;
}
