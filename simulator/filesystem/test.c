/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/15 20:26:14
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
#include  <time.h> 
#include  "types.h" 
#include  "fs.h" 

char *vfsPath = "vfs"; 
/*char *vfsPath; */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, 
    char **argv) {
  printf("%ld\n", sizeof(INODE)); 
  /*printf("%ld\n", sizeof(time_t)); */
  /*char c = 0; */
  /*printf("%d\t%d\t%d\n", c, ~c, ~~c); */

  printf("%d\n", (int)AMOUNT_OF_ADDRESS_PER_BLOCK); 
  printf("%d\n", (int)AMOUNT_OF_DIRENTRY_PER_BLOCK); 

  printf("%ld\n", sizeof(DIRENTRY)); 
  printf("block size = %d\n", (int)AMOUNT_OF_ALLBLOCK); 

  formatFS("_vhd"); 

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
