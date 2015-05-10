/*
 * =====================================================================================
 *
 *       Filename:  _writeStruct.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/03/15 22:01:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include  <stdio.h> 
#include	<stdlib.h>
#include  "_types.h"   
#include  <string.h> 
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
  data[0].nodeIndex = 0; 
  strcpy(data[0].buf, "data_0"); 
  data[1].nodeIndex = 1; 
  strcpy(data[1].buf, "data_1"); 

  if (NULL != (fp = fopen("data.txt", "w+"))) {
    fwrite(data, sizeof(TA), 2, fp);  
    fclose(fp); 
  }
  
  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
