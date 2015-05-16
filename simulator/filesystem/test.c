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
#include  <unistd.h> 
#include  <fcntl.h> 

char *vfsPath = "vfs"; 
PartitionTable *partitionTable; 
DIRENTRY workingDir; 
DIRENTRY rootEntry; 
INODE workingDirINODE; 
USER currentUser; 
GROUP currentGroup; 
char EMSG[1024]; 
char buf[1024]; 
int fd; 
int foo; 

/* 
 * 路径分割后的字符串数组 
 * */
char **pathArray; 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, 
    char **argv) {
  pathArray = (char **) malloc(sizeof(char *) * PATH_DEPTH); 
  for (foo = 0; foo < PATH_DEPTH; foo++) {
    *(pathArray + foo) = (char *) malloc(sizeof(char) * MAX_LENGTH_OF_FILENAME); 
  }

  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  formatFS(); 
  fd = open(vfsPath, O_RDWR); 
  partitionTable = loadPartitionTable(); 

  /*printf("%o\n", FULL_PERMISSION ^ UMASK_OF_FILE | (1 << 9)); */

  printf("创建根目录 -- %s\n", (-1 == createRootDirectory()) ? strFailure : strSuccess); 

  setRootEntry(); 
  workingDir = rootEntry; 
  workingDirINODE = loadINODE(workingDir.inodeNum);  

  _ls(); 

  printf("创建grp文件 -- %s\n", (-1 == createGrpFile()) ? strFailure : strSuccess); 

  _ls(); 

  printf("创建passwd文件 -- %s\n", (-1 == createPasswdFile()) ? strFailure : strSuccess); 

  _ls(); 

  _mkdir("dir1"); 
  _mkdir("dir2"); 
  _ls(); 

  DIRENTRY entry; 
  printf("grp%s\n", (0 == checkEntryExist("grp", &entry)) ? "不存在" : "已存在"); 

  _cd("dir1"); 
  _ls(); 
  
  /*_cd(".."); */
  /*_ls(); */

  if (-1 == _cd("grp")) {
    printEMSG();    
  }
  _ls(); 

  if (-1 == _cdl("/dir1/...") ) {
    printEMSG(); 
  }
  _ls(); 

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
