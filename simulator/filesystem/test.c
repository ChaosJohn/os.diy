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
#include  "utils.h" 
#include  <string.h> 

char *vfsPath = "vfs"; 
PartitionTable *partitionTable; 
DIRENTRY workingDir; 
INODE workingDirINODE; 
USER currentUser; 
/*DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; */

FILE *fp; 

/*char *strFailure = "FAILURE"; */
/*char *strSuccess = "SUCCESS"; */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, 
    char **argv) {
  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  formatFS(); 
  partitionTable = loadPartitionTable(); 

  /*printf("%o\n", FULL_PERMISSION ^ UMASK_OF_FILE | (1 << 9)); */

  printf("创建根目录 -- %s\n", (-1 == createRootDirectory()) ? strFailure : strSuccess); 

  workingDir.inodeNum = 0; 
  strcpy(workingDir.fileName, "/"); 
  workingDirINODE = loadINODE(workingDir.inodeNum);  
  
  _ls(); 

  printf("创建grp文件 -- %s\n", (-1 == createGrpFile()) ? strFailure : strSuccess); 
  printf("创建passwd文件 -- %s\n", (-1 == createPasswdFile()) ? strFailure : strSuccess); 


  fp = openFSForRead(); 
  if (NULL == fp) {
    return EXIT_FAILURE; 
  } else {
    fseek(fp, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
    fread(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); 
    fflush(fp); 
    fclose(fp); 
    int amountOfEntries = workingDirINODE.size / sizeof(DIRENTRY); 
    printf("%d\n", amountOfEntries); 
    int foo = 0; 
    for (; foo < amountOfEntries; foo++) {
      printf("%d: %s\n", dirEntries[foo].inodeNum, dirEntries[foo].fileName); 
    }
  }

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
