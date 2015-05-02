/*#include <stdio.h> */
/*#include <unistd.h> */
/*#include <stdarg.h> */
/*#include "vsprintf.c" */

#define PAGE_SIZE 4096 

static char printbuf[1024]; 

long user_stack [ PAGE_SIZE>>2 ] ;

/*extern int vsprintf(); */

struct {
  long * a;
  short b;
} stack_start = { & user_stack [PAGE_SIZE>>2] , 0x10 };

/*static int printf(const char *fmt, ...) {*/
/*va_list args; */
/*int i; */
/*va_start(args, fmt); */
/*write(1, printbuf, i = vsprintf(printbuf, fmt, args)); */
/*va_end(args); */
/*return i; */
/*} */

/*#define TM_PRINTF(f_, ...) printf((f_), __VA_ARGS__) */

char *str = "Helloworld!/n";  
void print()  
{  
  __asm__( "movl $13,%%edx \n\t"  
      "movl %0,%%ecx \n\t"  
      "movl $0,%%ebx \n\t"  
      "movl $4,%%eax \n\t"  
      "int $0x80 \n\t"  
      ::"r"(str):"edx","ecx","ebx");  
}  

void main(void) {
  /*printf("hhh\n"); */
  /*vsprintf(*/
  /*printf("hhh"); */
  print(); 
  return; 
}
