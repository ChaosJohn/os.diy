/*
 * =====================================================================================
 *
 *       Filename:  writeStruct.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/03/15 12:38:00
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
#include <string.h> 

typedef struct {
  char c;
  int h;
  short n;
  long m;
  float f;
  double d1;
  char s[100];
  double d2;
}st;
int main(void)
{
  FILE *fp;
  st sa,sb;
  char *str="abcdefg";
  sa.c='K';
  sa.h=-3;
  sa.n=20;
  sa.m=100000000;
  sa.f=33.32f;
  sa.d1=78.572;
  sa.d2=33.637;
  /*sa.s=str;*/
  strcpy(sa.s, str); 
  fp=fopen("st.txt","w+");
  if(!fp)
  {
    printf("errror!\n");
    exit(-1);
  }
  printf("sa:c=%c,h=%d,n=%d,m=%d,f=%f,d1=%f,s=%s,d2=%f\n",sa.c,sa.h,sa.n,sa.m,sa.f,sa.d1,sa.s,sa.d2);
  printf("sizeof(sa)=%d:&c=%x,&h=%x,&n=%x,&m=%x,&f=%x,&d1=%x,&s=%x,&d2=%x\n",sizeof(sa),&sa.c,&sa.h,&sa.n,&sa.m,&sa.f,&sa.d1,&sa.s,&sa.d2);
  fwrite(&sa,sizeof(sa),1,fp);
  rewind(fp);
  fread(&sb,sizeof(sb),1,fp);
  printf("sb:c=%c,h=%d,n=%d,m=%d,f=%f,d1=%f,s=%s,d2=%f\n",sb.c,sb.h,sb.n,sb.m,sb.f,sb.d1,sb.s,sb.d2);

  fclose(fp);

  return 0;
}
