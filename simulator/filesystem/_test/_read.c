/*
 * =====================================================================================
 *
 *       Filename:  _read.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/03/15 22:38:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include  <stdio.h> 
#include  "_types.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, 
    char **argv) {
  FILE *fp; 
  TA data[2]; 
  TA d; 
  TA a; 

  if (NULL != (fp = fopen("data.txt", "r"))) {
    fread(data, sizeof(TA), 2, fp); 
    rewind(fp); 
    fread(&d, sizeof(TA), 1, fp); 
    rewind(fp); 
    fseek(fp, sizeof(TA), SEEK_SET); 
    fread(&a, sizeof(TA), 1, fp); 
    fclose(fp); 
    int i = 0; 
    for (; i < 2; i++) {
      printf("%d, %s\n", data[i].nodeIndex, data[i].buf); 
    }
    printf("%d, %s\n", d.nodeIndex, d.buf); 
    printf("%d, %s\n", a.nodeIndex, a.buf); 

  }

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
