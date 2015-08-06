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
#include  "permission.h" 
#include  "cmd.h" 

char *vfsPath = "vfs"; 
PartitionTable *partitionTable; 
DIRENTRY workingDir; 
DIRENTRY rootEntry; 
INODE workingDirINODE; 
USER rootUser, currentUser; 
GROUP rootGroup, currentGroup; 
USER users[AMOUNT_OF_USER_PER_BLOCK]; 
GROUP groups[AMOUNT_OF_GROUP_PER_BLOCK]; 
GROUP theGroups[MAX_AMOUNT_OF_GROUPS_PER_USER]; 
char EMSG[1024]; 
char buf[1024]; 
int fd; 
int foo; 

char cmdline[1024]; 

/* 
 * 路径分割后的字符串数组 
 * */
char **pathArray; 

/* 
 * 命令分割后的字符串数组
 * */
char **cmdArray; 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, 
    char **argv) {
  initTables(); 
  for (foo = 1; foo < 6; foo++) {
    ffMalloc(foo * 20); 
  }

  // 构造默认的登入用户以及所属群组 
  currentUser.uid = 0; 
  sprintf(currentUser.name, "root"); 
  currentGroup.gid = 0; 
  sprintf(currentGroup.name, "root"); 
  rootUser = currentUser; 
  rootGroup = currentGroup; 
  // 初始化路径分割后存放的数组 
  pathArray = (char **) malloc(sizeof(char *) * PATH_DEPTH); 
  for (foo = 0; foo < PATH_DEPTH; foo++) {
    *(pathArray + foo) = (char *) malloc(sizeof(char) * MAX_LENGTH_OF_FILENAME); 
  }
  // 初始化命令分割后存放的数组
  cmdArray = (char **) malloc(sizeof(char *) * 32); 
  for (foo = 0; foo < 32; foo++) {
    *(cmdArray + foo) = (char *)malloc(sizeof(char) * 32); 
  }

  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  /*formatFS(); */
  fd = open(vfsPath, O_RDWR); 
  partitionTable = loadPartitionTable(); 

  /*printf("%o\n", FULL_PERMISSION ^ UMASK_OF_FILE | (1 << 9)); */

  /*printf("创建根目录 -- %s\n", (-1 == createRootDirectory()) ? strFailure : strSuccess); */

  setRootEntry(); 
  workingDir = rootEntry; 
  workingDirINODE = loadINODE(workingDir.inodeNum);  

  /*printf("创建grp文件 -- %s\n", (-1 == createGrpFile()) ? strFailure : strSuccess); */

  /*printf("创建passwd文件 -- %s\n", (-1 == createPasswdFile()) ? strFailure : strSuccess); */

  /*_mkdir("dir1"); */
  /*_mkdir("dir2"); */

  DIRENTRY entry; 

  /*if (-1 == addGroup("chaos")) {*/
    /*printEMSG(); */
  /*}*/

  /*if (-1 == addUserWithDefaultGroup("chaos", "chaos")) {*/
    /*printEMSG(); */
  /*}*/

  /*if (-1 == _touch("file1")) {*/
    /*printEMSG(); */
  /*}*/

  _su("chaos"); 

  /*printCmds(0, NULL); */

  while (1) {
    printf("[%s]%c ", currentUser.name, (0 == currentUser.uid) ? '#' : '$'); 
    memset(cmdline, 0, 1024); 
    fgets(cmdline, 1024, stdin); 
    if (-1 == executeCmd(cmdline)) {
      printEMSG(); 
    }
  } 
  
  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
