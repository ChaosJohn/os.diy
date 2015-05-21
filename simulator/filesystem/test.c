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
  /*printf("grp%s\n", (0 == checkEntryExist("grp", &entry)) ? "不存在" : "已存在"); */

  /*_cd("dir1"); */
  /*_ls(); */

  /*_cd(".."); */
  /*_ls(); */

  /*if (-1 == _cd("grp")) {*/
  /*printEMSG();    */
  /*}*/
  /*_ls(); */

  /*if (-1 == _cdl("/dir1/...") ) {*/
    /*printEMSG(); */
  /*}*/
  /*_ls(); */

  /*GROUP testGroup; */
  /*printf("%d\n", checkGroupExistByName("root", &testGroup)); */
  /*printf("%d\n", checkGroupExistByGid(1, &testGroup)); */

  /*showAllGroups(); */

  if (-1 == addGroup("chaos")) {
    printEMSG(); 
  }
  showAllGroups(); 

  if (-1 == deleteGroupByName("root")) {
    printEMSG(); 
  }
  showAllGroups(); 

  if (-1 == deleteGroupByGid(0)) {
    printEMSG(); 
  }
  showAllGroups(); 

  if (-1 == showAllUsers()) {
    printEMSG(); 
  }

  if (-1 == deleteUserByName("root")) {
    printEMSG(); 
  }

  showAllUsers(); 

  if (-1 == deleteUserByUid(0)) {
    printEMSG(); 
  }

  if (-1 == addUserWithDefaultGroup("chaos", "chaos")) {
    printEMSG(); 
  }
  showAllUsers(); 

  if (-1 == _touch("file1")) {
    printEMSG(); 
  }
  _ls(); 

  if (-1 == _touch("file1")) {
    printEMSG(); 
  }
  _ls(); 

  _whoami(); 

  _su("chaos"); 
  _whoami(); 

  /*_passwd("chaos"); */
  /*_su("chaos"); */
  /*_passwd("chaos"); */
  /*_su("chaos"); */

  /*printf("%s %s\n", cmds[0].command, cmds[0].description); */
  printCmds(0, NULL); 

  /*char cmd1[] = "touch newFile"; */
  /*int argCount = splitCmd(cmd1); */
  /*cmds[2].execute(argCount, cmdArray); */
  /*_ls(); */
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
