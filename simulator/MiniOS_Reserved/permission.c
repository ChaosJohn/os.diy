/*
 * =====================================================================================
 *
 *       Filename:  permission.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/15/15 21:36:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include  "permission.h" 

USER 
genEmptyUser() {
  USER user; 
  memset(user.gids, -1, 15); 
}

int 
checkGroupExistByName(char *name, 
    GROUP *group) {
  int groupAmount; 
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  } else {
    int foo = 0; 
    for (; foo < groupAmount; foo++) {
      if (1 == compareString(groups[foo].name, name)) {
        *group = groups[foo]; 
        return 1; 
      }
    }
    return 0; 
  }
}

int 
checkGroupExistByGid(char gid, 
    GROUP *group) {
  int groupAmount; 
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  } else {
    int foo = 0; 
    for (; foo < groupAmount; foo++) {
      if (gid == groups[foo].gid) {
        *group = groups[foo]; 
        return 1; 
      }
    }
    return 0; 
  }
}

int 
checkUserExistByName(char *name, 
    USER *user) {
  int userAmount; 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  } else {
    int foo = 0; 
    for (; foo < userAmount; foo++) {
      if (1 == compareString(users[foo].name, name)) {
        *user = users[foo]; 
        return 1; 
      }
    }
    return 0; 
  }
}

int 
checkUserExistByUid(char uid, 
    USER *user) {
  int userAmount; 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  } else {
    int foo = 0; 
    for (; foo < userAmount; foo++) {
      if (uid == users[foo].uid) {
        *user = users[foo]; 
        return 1; 
      }
    }
    return 0; 
  }
}

int 
loadGrpFile(int *groupAmount) {
  DIRENTRY grpFileEntry; 
  if (-1 == findFileByFilePath("/grp", &grpFileEntry)) {
    sprintf(EMSG, "没有找到群组文件"); 
    return -1; 
  } else {
    INODE grpFileINODE = loadINODE(grpFileEntry.inodeNum); 
    lseek(fd, getActualAddressOfDataBlock(grpFileINODE.headAddress), SEEK_SET); 
    read(fd, groups, AMOUNT_OF_GROUP_PER_BLOCK * sizeof(GROUP)); 
    *groupAmount = grpFileINODE.size / sizeof(GROUP); 
    return 0; 
  } 
}

int 
loadPasswdFile(int *userAmount) {
  DIRENTRY passwdFileEntry; 
  if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
    sprintf(EMSG, "没有找到密码文件"); 
    return -1; 
  } else {
    INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
    lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
    read(fd, users, AMOUNT_OF_USER_PER_BLOCK * sizeof(USER)); 
    *userAmount = passwdFileINODE.size / sizeof(USER); 
  }
}

int 
showAllGroups() {
  int groupAmount; 
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  } else {
    printf("Amount of groups = %d\n", groupAmount); 
    int foo = 0; 
    for (; foo < groupAmount; foo++) {
      printf("#%-3d: %s", groups[foo].gid, groups[foo].name); 
      println(); 
    }
    return 0; 
  }
}

int 
showAllUsers() {
  int userAmount; 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  } else {
    printf("Amount of users = %d\n", userAmount); 
    int foo = 0; 
    for (; foo < userAmount; foo++) {
      printf("#%-3d: %s", users[foo].uid, users[foo].name);
      println(); 
    }
    return 0; 
  }
}

int 
deleteGroupByName(char *name) {
  if (1 == compareString(name, "root")) {
    sprintf(EMSG, "不能删除root群组"); 
    return -1; 
  }
  int groupAmount; 
  // 从grp文件中加载到groups数组中
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  }
  // 检查是否存在该群组
  GROUP group; 
  if (1 != checkGroupExistByName(name, &group)) {
    return -1; 
  } else {
    GROUP newGroups[AMOUNT_OF_GROUP_PER_BLOCK] = {0}; 
    int newIndex = 0; 
    int foo = 0; 
    for (; foo < groupAmount; foo++) {
      if (1 == compareString(groups[foo].name, name)) {
        continue; 
      } else {
        newGroups[newIndex] = groups[foo]; 
        newIndex++; 
      }
    }
    DIRENTRY grpFileEntry; 
    if (-1 == findFileByFilePath("/grp" ,&grpFileEntry)) {
      return -1; 
    } else {
      INODE grpFileINODE = loadINODE(grpFileEntry.inodeNum); 
      lseek(fd, getActualAddressOfDataBlock(grpFileINODE.headAddress), SEEK_SET); 
      write(fd, newGroups, AMOUNT_OF_GROUP_PER_BLOCK * sizeof(GROUP)); 
      grpFileINODE.size = newIndex * sizeof(GROUP); 
      lseek(fd, getActualAddressOfINODE(grpFileEntry.inodeNum), SEEK_SET); 
      write(fd, &grpFileINODE, sizeof(INODE)); 
      return 0; 
    }
  }
}

int 
deleteUserByName(char *name) {
  if (1 == compareString(name, "root")) {
    sprintf(EMSG, "不能删除root用户"); 
    return -1; 
  }
  int userAmount; 
  // 从passwd文件中加载到users数组中 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  }
  // 检查是否存在该用户
  USER user; 
  if (1 != checkUserExistByName(name, &user)) {
    return -1; 
  } else {
    USER newUsers[AMOUNT_OF_USER_PER_BLOCK] = {0}; 
    int newIndex = 0; 
    int foo = 0; 
    for (; foo < userAmount; foo++) {
      if (1 == compareString(users[foo].name, name)) {
        continue; 
      } else {
        newUsers[newIndex] = users[foo]; 
        newIndex++; 
      }
    }
    DIRENTRY passwdFileEntry; 
    if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
      return -1; 
    } else {
      INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
      lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
      write(fd, newUsers, AMOUNT_OF_USER_PER_BLOCK * sizeof(USER)); 
      passwdFileINODE.size = newIndex * sizeof(USER); 
      lseek(fd, getActualAddressOfINODE(passwdFileEntry.inodeNum), SEEK_SET); 
      write(fd, &passwdFileINODE, sizeof(INODE)); 
      return 0; 
    }
  }
}

int 
deleteGroupByGid(char gid) {
  if (0 == gid) {
    sprintf(EMSG, "不能删除root群组"); 
    return -1; 
  }
  int groupAmount; 
  // 从grp文件中加载到groups数组中
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  }
  // 检查是否存在该群组
  GROUP group; 
  if (1 != checkGroupExistByGid(gid, &group)) {
    return -1; 
  } else {
    GROUP newGroups[AMOUNT_OF_GROUP_PER_BLOCK] = {0}; 
    int newIndex = 0; 
    int foo = 0; 
    for (; foo < groupAmount; foo++) {
      if (gid == groups[foo].gid) {
        continue; 
      } else {
        newGroups[newIndex] = groups[foo]; 
        newIndex++; 
      }
    }
    DIRENTRY grpFileEntry; 
    if (-1 == findFileByFilePath("/grp" ,&grpFileEntry)) {
      return -1; 
    } else {
      INODE grpFileINODE = loadINODE(grpFileEntry.inodeNum); 
      lseek(fd, getActualAddressOfDataBlock(grpFileINODE.headAddress), SEEK_SET); 
      write(fd, newGroups, AMOUNT_OF_GROUP_PER_BLOCK * sizeof(GROUP)); 
      grpFileINODE.size = newIndex * sizeof(GROUP); 
      lseek(fd, getActualAddressOfINODE(grpFileEntry.inodeNum), SEEK_SET); 
      write(fd, &grpFileINODE, sizeof(INODE)); 
      return 0; 
    }
  }
}

int 
deleteUserByUid(char uid) {
  if (0 == uid) {
    sprintf(EMSG, "不能删除root用户"); 
    return -1; 
  }
  int userAmount; 
  // 从passwd文件中加载到users数组中 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  }
  // 检查是否存在该用户
  USER user; 
  if (1 != checkUserExistByUid(uid, &user)) {
    return -1; 
  } else {
    USER newUsers[AMOUNT_OF_USER_PER_BLOCK] = {0}; 
    int newIndex = 0; 
    int foo = 0; 
    for (; foo < userAmount; foo++) {
      if (users[foo].uid== uid) {
        continue; 
      } else {
        newUsers[newIndex] = users[foo]; 
        newIndex++; 
      }
    }
    DIRENTRY passwdFileEntry; 
    if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
      return -1; 
    } else {
      INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
      lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
      write(fd, newUsers, AMOUNT_OF_USER_PER_BLOCK * sizeof(USER)); 
      passwdFileINODE.size = newIndex * sizeof(USER); 
      lseek(fd, getActualAddressOfINODE(passwdFileEntry.inodeNum), SEEK_SET); 
      write(fd, &passwdFileINODE, sizeof(INODE)); 
      return 0; 
    }
  }
}

int 
addGroup(char *name) {
  GROUP group; 
  if (0 != checkGroupExistByName(name, &group)) {
    resetEMSG();
    sprintf(EMSG, "无法创建%s群组", name); 
    return -1; 
  }
  int groupAmount; 
  // 从grp文件中加载到groups数组中
  if (-1 == loadGrpFile(&groupAmount)) {
    return -1; 
  }
  GROUP newGroup; 
  sprintf(newGroup.name, "%s", name); 
  newGroup.gid = groups[groupAmount - 1].gid + 1; 
  groups[groupAmount] = newGroup; 
  groupAmount++; 
  DIRENTRY grpFileEntry; 
  if (-1 == findFileByFilePath("/grp", &grpFileEntry)) {
    return -1; 
  } else {
    INODE grpFileINODE = loadINODE(grpFileEntry.inodeNum); 
    lseek(fd, getActualAddressOfDataBlock(grpFileINODE.headAddress), SEEK_SET); 
    write(fd, groups, AMOUNT_OF_GROUP_PER_BLOCK * sizeof(GROUP)); 
    grpFileINODE.size = groupAmount * sizeof(GROUP); 
    lseek(fd, getActualAddressOfINODE(grpFileEntry.inodeNum), SEEK_SET); 
    write(fd, &grpFileINODE, sizeof(INODE)); 
    return 0; 
  }
}

int 
addUserWithDefaultGroup(char *userName, 
    char *groupName) {
  USER user; 
  if (0 != checkUserExistByName(userName, &user)) {
    resetEMSG(); 
    sprintf(EMSG, "无法创建%s用户", userName); 
    return -1; 
  }
  GROUP group; 
  if (1 != checkGroupExistByName(groupName, &group)) {
    resetEMSG();
    sprintf(EMSG, "无法找到%s群组", groupName); 
    return -1; 
  }
  int userAmount; 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  }
  USER newUser; 
  sprintf(newUser.name, "%s", userName); 
  newUser.uid = users[userAmount - 1].uid + 1; 
  memset(newUser.gids, -1, MAX_AMOUNT_OF_GROUPS_PER_USER); 
  memset(newUser.password, -1, MAX_LENGTH_OF_PASSWORD); 
  newUser.gids[0] = group.gid; 
  users[userAmount] = newUser; 
  userAmount++; 
  DIRENTRY passwdFileEntry; 
  if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
    return -1; 
  } else {
    INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
    lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
    write(fd, users, AMOUNT_OF_USER_PER_BLOCK * sizeof(USER)); 
    passwdFileINODE.size = userAmount * sizeof(USER); 
    lseek(fd, getActualAddressOfINODE(passwdFileEntry.inodeNum), SEEK_SET); 
    write(fd, &passwdFileINODE, sizeof(INODE)); 
    return 0; 
  }
}

int 
joinGroup(char *userName, 
    char *groupName) {
  USER user; 
  if (NULL == userName) {
    user = currentUser; 
  } else if (1 != checkUserExistByName(userName, &user)) {
    resetEMSG(); 
    sprintf(EMSG, "未找到%s用户", userName); 
    return -1; 
  }
  GROUP group; 
  if (1 != checkGroupExistByName(groupName, &group)) {
    resetEMSG(); 
    sprintf(EMSG, "未找到%s群组", groupName); 
    return -1; 
  }
  int userAmount; 
  loadPasswdFile(&userAmount); 
  int foo = 0; 
  for (; foo < userAmount; foo++) {
    if (user.uid != users[foo].uid) {
      continue; 
    } else {
      user = users[foo]; 
      int gidIndex = 1; 
      while (gidIndex < MAX_AMOUNT_OF_GROUPS_PER_USER) {
        if (-1 == user.gids[gidIndex]) {
          user.gids[gidIndex] = group.gid; 
          /*printf("%d, %d\n", gidIndex, group.gid); */
          break; 
        }
        gidIndex++; 
      }
      users[foo] = user; 
      break; 
    }
  }
  DIRENTRY passwdFileEntry; 
  if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
    return -1; 
  }
  INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
  lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
  write(fd, users, AMOUNT_OF_USER_PER_BLOCK * sizeof(USER)); 
  return 0; 
}

void 
setUid(INODE *inode, 
    char uid) {
  char *permissionAddr = (char *)&(inode->permission); 
  *(permissionAddr + 2) = uid; 
}

char  
getUid(INODE inode) {
  char *permissionAddr = (char *)&(inode.permission); 
  return *(permissionAddr + 2); 
}

int 
_chown(char *fileName, 
    char *userName) {
  DIRENTRY entry; 
  if (1 != checkEntryExist(fileName, &entry)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s文件", fileName); 
    return -1; 
  }
  USER user; 
  if (1 != checkUserExistByName(userName, &user)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s用户", userName); 
    return -1; 
  }
  INODE inode; 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  read(fd, &inode, sizeof(INODE)); 
  /*char *permissionAddr = (char *)&(inode.permission); */
  /**(permissionAddr + 2) = uid; */
  setUid(&inode, user.uid); 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  write(fd, &inode, sizeof(INODE)); 
  return 0; 
}

char 
_getUid(char *fileName) {
  DIRENTRY entry; 
  if (1 != checkEntryExist(fileName, &entry)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s文件", fileName); 
    return -1; 
  }
  INODE inode; 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  read(fd, &inode, sizeof(INODE)); 
  return getUid(inode); 
}

void 
setGid(INODE *inode, 
    char gid) {
  char *permissionAddr = (char *)&(inode->permission); 
  *(permissionAddr + 3) = gid; 
}

char  
getGid(INODE inode) {
  char *permissionAddr = (char *)&(inode.permission); 
  return *(permissionAddr + 3); 
}

int 
_chgrp(char *fileName, 
    char *groupName) {
  DIRENTRY entry; 
  if (1 != checkEntryExist(fileName, &entry)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s文件", fileName); 
    return -1; 
  }
  GROUP group; 
  if (1 != checkGroupExistByName(groupName, &group)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s群组", groupName); 
    return -1; 
  }
  INODE inode; 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  read(fd, &inode, sizeof(INODE)); 
  /*char *permissionAddr = (char *)&(inode.permission); */
  /**(permissionAddr + 3) = gid; */
  setGid(&inode, group.gid); 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  write(fd, &inode, sizeof(INODE)); 
  return 0; 
}

char 
_getGid(char *fileName) {
  DIRENTRY entry; 
  if (1 != checkEntryExist(fileName, &entry)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s文件", fileName); 
    return -1; 
  }
  INODE inode; 
  lseek(fd, getActualAddressOfINODE(entry.inodeNum), SEEK_SET); 
  read(fd, &inode, sizeof(INODE)); 
  return getGid(inode); 
}

int 
getTheGroups(char *userName, 
    int *groupAmount) {
  USER user; 
  if (NULL == userName) {
    user = currentUser; 
  } else if (1 != checkUserExistByName(userName, &user)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s用户", userName); 
    return -1; 
  }
  int gidIndex = 0; 
  GROUP group; 
  while (gidIndex < MAX_AMOUNT_OF_GROUPS_PER_USER) {
    if (-1 != user.gids[gidIndex]) {
      if (1 != checkGroupExistByGid(user.gids[gidIndex], &group)) {
        theGroups[gidIndex].gid = user.gids[gidIndex]; 
        sprintf(theGroups[gidIndex].name, "unknown"); 
      } else {
        theGroups[gidIndex] = group; 
      }
      gidIndex++; 
    } else {
      break; 
    }
  }
  *groupAmount = gidIndex; 
  printf("%d\n", gidIndex); 
  return 0; 
}

int 
showGroups(char *userName) {
  int groupAmount; 
  if (-1 == getTheGroups(userName, &groupAmount)) {
    return -1; 
  }
  int foo = 0; 
  for (; foo < groupAmount; foo++) {
    printf("%s\n", theGroups[foo].name); 
  }
  return 0; 
}

int 
_su(char *userName) {
  USER user; 
  if (1 != checkUserExistByName(userName, &user)) {
    resetEMSG(); 
    sprintf(EMSG, "没有找到%s用户", userName); 
    return -1; 
  }
  if (user.uid == 0) {
    // 对root用户跳过验证
  } else if (-1 != user.password[0]) {
    printf("请输入密码: "); 
    char *password = (char *)malloc(MAX_LENGTH_OF_PASSWORD * sizeof(char));
    fgets(password, MAX_LENGTH_OF_PASSWORD, stdin); 
    if (0 == compareString(user.password, password)) {
      resetEMSG(); 
      sprintf(EMSG, "密码错误"); 
      return -1; 
    }
  }
  currentUser = user; 
  if (1 != checkGroupExistByGid(user.gids[0], &currentGroup)) {
    currentGroup = rootGroup; 
  }
  return 0; 
}

void 
_whoami() {
  printf("%s\n", currentUser.name); 
}

int 
_passwd(char *userName) {
  USER user; 
  if (NULL == userName) {
    user = currentUser; 
  } else {
    if (1 != checkUserExistByName(userName, &user)) {
      resetEMSG(); 
      sprintf(EMSG, "没有找到%s用户", userName); 
      return -1; 
    }
  } 
  if (currentUser.uid == 0) {
    // root用户，跳过旧密码验证
  } else if (-1 != user.password[0]) {
    printf("请输入旧密码: "); 
    char *password = (char *)malloc(MAX_LENGTH_OF_PASSWORD * sizeof(char));
    fgets(password, MAX_LENGTH_OF_PASSWORD, stdin); 
    if (0 == compareString(user.password, password)) {
      resetEMSG(); 
      sprintf(EMSG, "密码错误"); 
      return -1; 
    }
  }
  printf("请输入新密码: "); 
  fgets(user.password, MAX_LENGTH_OF_PASSWORD, stdin); 
  _updateUser(user); 
  if (NULL == userName) {
    currentUser = user; 
  }
  printf("密码已更新\n"); 
  return 0; 
}

int 
_updateUser(USER user) {
  int userAmount; 
  if (-1 == loadPasswdFile(&userAmount)) {
    return -1; 
  }
  int foo = 0; 
  for (; foo < userAmount; foo++) {
    if (users[foo].uid == user.uid) {
      users[foo] = user; 
      /*printf("%d %s\n", users[foo].uid, users[foo].password); */
      break; 
    }
  }
  DIRENTRY passwdFileEntry; 
  if (-1 == findFileByFilePath("/passwd", &passwdFileEntry)) {
    return -1; 
  }
  INODE passwdFileINODE = loadINODE(passwdFileEntry.inodeNum); 
  lseek(fd, getActualAddressOfDataBlock(passwdFileINODE.headAddress), SEEK_SET); 
  write(fd, users, AMOUNT_OF_USER_PER_BLOCK); 
  return 0; 
}
