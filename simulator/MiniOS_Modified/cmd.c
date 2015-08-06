/*
 * =====================================================================================
 *
 *       Filename:  cmd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/19/15 15:56:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "cmd.h" 

CMD cmds[100] = {
  {"help", "帮助", &printCmds}, 
  {"quit", "退出", &quit}, 
  {"touch", "创建文件或修正文件时间", &touch}, 
  {"mkdir", "创建文件夹", &mkdir}, 
  {"ls", "列出当前目录的文件清单", &ls}, 
  {"ll", "按长格式列出当前目录的文件清单", &ll}, 
  {"chown", "更改文件的所有者", &chowner}, 
  {"chgrp", "更改文件的所属群组", &chgroup}, 
  {"groupadd", "新增群组", &groupadd}, 
  {"useradd", "新增用户", &useradd}, 
  {"passwd", "更新用户密码", &passwd}, 
  {"su", "切换用户", &su}, 
  {"whoami", "查看当前用户", &whoami}, 
  {"joingroup", "加入群组", &joingroup}, 
  {"showgroups", "显示用户的所有群组", &showgroups}, 
  {"lsusers", "显示所有用户", &lsusers}, 
  {"lsgroups", "显示所有群组", &lsgroups}, 
  {"cd", "更改目录", &cd}, 
  {"ffmalloc", "使用首次适应算法分配内存", &ffmalloc}, 
  {"nfmalloc", "使用循环首次适应算法分配内存", &nfmalloc}, 
  {"bfmalloc", "使用最佳适应算法分配内存", &bfmalloc}, 
  {"wfmalloc", "使用最差适应算法分配内存", &wfmalloc}, 
  {"showmm", "显示内存空闲表和内存分配表", &showmm}, 
  {"showempty", "显示内存空闲表", &showempty}, 
  {"showused", "显示内存分配表", &showused}, 
  {"free", "回收内存", &freemm}, 
  {"runjobs", "模拟多任务", &runjobs}, 
}; 

int 
executeCmd(char *cmdline) {
  int argc = splitCmd(cmdline); 
  int foo = 0; 
  for (; foo < 100; foo++) {
    if (1 == compareString(cmds[foo].command, cmdArray[0])) {
      return cmds[foo].execute(argc, cmdArray); 
    }
  }
  resetEMSG(); 
  sprintf(EMSG, "没有找到%s命令", cmdArray[0]); 
  return -1; 
}

int  
printCmds(int argc,
    char **argv) {
  /*println(); */
  int foo = 0; 
  for (; foo < 100; foo++) {
    if (cmds[foo].command[0] == 0) {
      break; 
    } else {
      printf("* %-16s%s\n", cmds[foo].command, cmds[foo].description); 
    }
  }
  return 0; 
}

int 
quit(int argc, 
    char **argv) {
  exit(EXIT_SUCCESS); 
}

int 
touch(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s file", argv[0]); 
    return -1; 
  }
  return _touch(argv[1]); 
}

int 
mkdir(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s folder", argv[0]); 
    return -1; 
  }
  return _mkdir(argv[1]); 
}

int 
ls(int argc, 
    char **argv) {
  _ls(); 
  return 0; 
}

int 
ll(int argc, 
    char **argv) {
  _ll(); 
  return 0; 
}

int 
chowner(int argc, 
    char **argv) {
  if (argc < 3) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s user file1 file2 ...", argv[0]); 
    return -1; 
  }
  int foo = 2; 
  for (; foo < argc; foo++) { 
    if (-1 == _chown(argv[foo], argv[1])) {
      return -1; 
    }
  } 
  return 0; 
}

int 
chgroup(int argc, 
    char **argv) {
  if (argc < 3) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s group file1 file2 ...", argv[0]); 
    return -1; 
  }
  int foo = 2; 
  for (; foo < argc; foo++) { 
    if (-1 == _chgrp(argv[foo], argv[1])) {
      return -1; 
    }
  } 
  return 0; 
}

int 
groupadd(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s group", argv[0]); 
    return -1; 
  }
  return addGroup(argv[1]); 
}

int 
useradd(int argc, 
    char **argv) {
  if (3 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s user default-group", argv[0]); 
    return -1; 
  }
  return addUserWithDefaultGroup(argv[1], argv[2]); 
}

int 
passwd(int argc, 
    char **argv) {
  if (argc < 2) {
    return _passwd(NULL); 
  } else {
    return _passwd(argv[1]); 
  }
}

int 
su(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s user", argv[0]); 
    return -1; 
  }
  return _su(argv[1]); 
}

int 
whoami(int argc, 
    char **argv) {
  _whoami(); 
  return 0; 
}

int 
joingroup(int argc, 
    char **argv) {
  if (2 == argc) {
    return joinGroup(NULL, argv[1]); 
  } else {
    int foo = 2; 
    for (; foo < argc; foo++) {
      if (-1 == joinGroup(argv[1], argv[foo])) {
        return -1; 
      }
    }
    return 0; 
  }
}

int 
showgroups(int argc, 
    char **argv) {
  if (1 == argc) { 
    return showGroups(NULL); 
  } else {
    /*int foo = 2; */
    /*for (; foo < argc; foo++) {*/
      /*if (-1 == showGroups(argv[foo])) {*/
        /*return -1; */
      /*}*/
    /*}*/
    /*return 0; */
    return showGroups(argv[1]); 
  }
}

int 
lsusers(int argc, 
    char **argv) {
  return showAllUsers(); 
}

int 
lsgroups(int argc, 
    char **argv) {
  return showAllGroups(); 
}

int 
cd(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s filePath", argv[0]); 
    return -1; 
  }
  return _cdl(argv[1]); 
}

int 
ffmalloc(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s size", argv[0]); 
    return -1; 
  }
  ffMalloc(atoi(argv[1])); 
  return 0; 
}

int 
nfmalloc(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s size", argv[0]); 
    return -1; 
  }
  nfMalloc(atoi(argv[1])); 
  return 0; 
}

int 
bfmalloc(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s size", argv[0]); 
    return -1; 
  }
  bfMalloc(atoi(argv[1])); 
  return 0; 
}

int 
wfmalloc(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s size", argv[0]); 
    return -1; 
  }
  wfMalloc(atoi(argv[1])); 
  return 0; 
}

int 
showmm(int argc, 
    char **argv) {
  displayEmptyTable(); 
  displayAllocatedTable(); 
  return 0; 
}

int 
showempty(int argc, 
    char **argv) {
  displayEmptyTable(); 
  return 0; 
}

int 
showused(int argc, 
    char **argv) {
  displayAllocatedTable(); 
  return 0; 
}

int 
freemm(int argc, 
    char **argv) {
  if (2 != argc) {
    resetEMSG(); 
    sprintf(EMSG, "[usage]: %s memory-num", argv[0]); 
    return -1; 
  }
  myFree(atoi(argv[1])); 
  return 0; 
}

int 
runjobs(int argc, 
    char **argv) {
  return scheduleJobs(); 
}
