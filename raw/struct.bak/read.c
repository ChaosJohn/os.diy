/*
 * =====================================================================================
 *
 *       Filename:  read.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/03/15 12:43:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char c;
  int h;
  short n;
  long m;
  float f;
  double d1;
  char *s;
  double d2;
}st;
int main(void)
{
  FILE *fp;
  st sb;
  fp=fopen("st.txt","r");
  if(!fp)
  {
    printf("errror!\n");
    exit(-1);
  }
  fread(&sb,sizeof(sb),1,fp);
  printf("sb:c=%c,h=%d,n=%d,m=%d,f=%f,d1=%f,s=%s,d2=%f\n",sb.c,sb.h,sb.n,sb.m,sb.f,sb.d1,sb.s,sb.d2);
  printf("sizeof(sb)=%d:&c=%x,&h=%x,&n=%x,&m=%x,&f=%x,&d1=%x,&s=%x,&d2=%x\n",sizeof(sb),&sb.c,&sb.h,&sb.n,&sb.m,&sb.f,&sb.d1,&sb.s,&sb.d2);

  fclose(fp);

  return 0;
}
