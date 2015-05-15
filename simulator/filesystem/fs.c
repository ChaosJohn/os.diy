/*
 * =====================================================================================
 *
 *       Filename:  fs.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/06/15 19:44:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include  "types.h" 
#include  <stdio.h> 
#include  "fs.h" 
#include  <string.h> 
#include  <fcntl.h> 
#include  <unistd.h> 

void 
formatFS() {
  /*extern char *fileName; */
  int fd;  
  if (-1 != (fd = open(vfsPath, O_WRONLY|O_CREAT|O_TRUNC))) {
    /*fseek(fp, SEEK_SET, AMOUNT_OF_ALLBLOCK * BLOCKSIZE + 1); */
    /*char buf[] = {EOF}; */
    /*fwrite(buf,1, 1, fp); */
    char buf[BLOCKSIZE] = {0}; 
    printf("格式化...\n"); 
    int foo = 0; 
    // 将整个分区置0 
    for (; foo < AMOUNT_OF_ALLBLOCK; foo++) {
      write(fd, buf, BLOCKSIZE); 
      /*fwrite(buf, 1, BLOCKSIZE, fp); */
    }
    printf("格式化完成!\n"); 

    lseek(fd, 0, SEEK_SET); 

    char availableFlag = 0; 

    // 初始化INODE空闲表
    printf("初始化INODE空闲表...\n"); 
    for (foo = 0; foo < AMOUNT_OF_INODE; foo++) {
      write(fd, &availableFlag, 1); 
    }
    printf("初始化INODE空闲表完成!\n"); 

    // 初始化数据block空闲表 
    printf("初始化数据block空闲表...\n"); 
    for (foo = 0; foo < AMOUNT_OF_DATABLOCK; foo++) {
      write(fd, &availableFlag, 1); 
    }
    printf("初始化数据block空闲表完成!\n"); 

    close(fd); 
  } 
}

PartitionTable * 
loadPartitionTable() {
  PartitionTable *partitionTable = (PartitionTable *)malloc(sizeof(PartitionTable)); 
  read(fd, partitionTable->tableINODE, sizeof(char) * AMOUNT_OF_INODE); 
  read(fd, partitionTable->tableDataBlock, sizeof(char) * AMOUNT_OF_DATABLOCK); 
  return partitionTable; 
}

int 
restorePartitionTable() {
  lseek(fd, 0, SEEK_SET); 
  write(fd, partitionTable->tableINODE, sizeof(char) * AMOUNT_OF_INODE); 
  write(fd, partitionTable->tableDataBlock, sizeof(char) * AMOUNT_OF_DATABLOCK); 
  return 0; 
}

int 
findAvailableINODE() {
  int index = 0; 
  for (; index < AMOUNT_OF_INODE; index++) {
    if (1 == partitionTable->tableINODE[index]) {
      continue; 
    } else {
      /*printf("available inodeNum: %d\n", index); */
      return index; 
    }
  }
  return -1; 
}

int 
findAvailableDataBlock() {
  int index = 0; 
  for (; index < AMOUNT_OF_DATABLOCK; index++) {
    if (1 == partitionTable->tableDataBlock[index]) {
      continue; 
    } else {
      /*printf("available datablock: %d\n", index); */
      return index; 
    }
  }
  return -1; 
}

int 
createRootDirectory() {
  int indexOfINODE = findAvailableINODE(); 
  if (-1 != indexOfINODE) {
    DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
    dirEntries[0].inodeNum = indexOfINODE; 
    /*memset(dirEntries[0].fileName, 0, 28); */
    sprintf(dirEntries[0].fileName, "."); 
    dirEntries[1].inodeNum = indexOfINODE; 
    /*memset(dirEntries[1].fileName, 0, 28); */
    sprintf(dirEntries[1].fileName, ".."); 
    INODE inode; 
    inode.permission = getDefaultPermission(1); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 2 * sizeof(DIRENTRY); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    // 将数据写回到datablock中
    /*printf("address: %d\t", getActualAddressOfDataBlock(inode.headAddress)); */
    /*printf("lseek: %ld\n", lseek(fd, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET)); */
    lseek(fd, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    write(fd, dirEntries, sizeof(DIRENTRY) * AMOUNT_OF_DIRENTRY_PER_BLOCK); 
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    return writeBackINODE(0, inode); 
  } else {
    return -1; 
  }
}

int 
getDefaultPermission(int type) {
  /* 
   * 将输入限定在目录和文件之间
   * 即type为1是目录，其余情况都为普通文件
   * */
  type = (1 == type) ? 1 : 0; 
  return (FULL_PERMISSION ^ (1 == type ? UMASK_OF_DIR : UMASK_OF_FILE)) | (type << 9); 
}

int 
writeBackINODE(int inodeNum, 
    INODE inode) {
  lseek(fd, getActualAddressOfINODE(inodeNum), SEEK_SET); 
  write(fd, &inode, sizeof(INODE)); 
  partitionTable->tableINODE[inodeNum] = 1; 
  return restorePartitionTable(partitionTable); 
}

ADDRESS  
getActualAddressOfINODE(int inodeNum) {
  ADDRESS actualAddress = 
    (AMOUNT_OF_BLOCK_FOR_TABLE_OF_INODE + AMOUNT_OF_BLOCK_FOR_TABLE_OF_DATABLOCK)  
    * BLOCKSIZE + sizeof(INODE) * inodeNum; 
  return actualAddress; 
}

FILE * 
openFSForWrite() {
  return fopen(vfsPath, "r+"); 
}

FILE *
openFSForRead() {
  return fopen(vfsPath, "r"); 
}

int 
createPasswdFile() {
  int inodeNum = findAvailableINODE(); 
  if (-1 != inodeNum) {
    USER userArray[AMOUNT_OF_USER_PER_BLOCK]; 
    USER user; 
    user.gid[0] = currentGroup.gid; 
    user.uid = 0; 
    sprintf(user.name, "root"); 
    INODE inode; 
    inode.permission = getDefaultPermission(0); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 1 * sizeof(USER); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    lseek(fd, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    write(fd, userArray, sizeof(USER)); 
    currentUser = user; 
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    // 在当前目录中写入新目录项
    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    sprintf(newEntry.fileName, "passwd"); 
    if (-1 == addDIRENTRY(newEntry)) {
      return -1; 
    } 
    return writeBackINODE(inodeNum, inode); 
  } else {
    return -1; 
  }
}

int 
createGrpFile() {
  int inodeNum = findAvailableINODE(); 
  if (-1 != inodeNum) {
    GROUP groupArray[AMOUNT_OF_GROUP_PER_BLOCK]; 
    GROUP group; 
    group.gid = 0; 
    sprintf(group.name, "root"); 
    groupArray[0] = group; 
    INODE inode; 
    inode.permission = getDefaultPermission(0); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 1 * sizeof(GROUP); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    write(fd, &group, sizeof(GROUP)); 
    currentGroup = group; 
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    // 在当前目录中写入新目录项
    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    sprintf(newEntry.fileName, "grp"); 
    if (-1 == addDIRENTRY(newEntry)) {
      return -1; 
    } 
    return writeBackINODE(inodeNum, inode); 
  } else {
    return -1; 
  }
}

ADDRESS 
getActualAddressOfDataBlock(int blockNum) {
  ADDRESS actualAddress = 
    (AMOUNT_OF_BLOCK_FOR_TABLE_OF_INODE + AMOUNT_OF_BLOCK_FOR_TABLE_OF_DATABLOCK) 
    * BLOCKSIZE + SPACE_OF_ALL_INODE + BLOCKSIZE * blockNum; 
  return actualAddress; 
}

int 
addDIRENTRY(DIRENTRY newEntry) {
  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  lseek(fd, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  read(fd, dirEntries, sizeof(DIRENTRY) * AMOUNT_OF_DIRENTRY_PER_BLOCK); 
  int amountOfEntries = workingDirINODE.size / sizeof(DIRENTRY); 
  dirEntries[amountOfEntries] = newEntry; 
  amountOfEntries++; 
  workingDirINODE.size += sizeof(DIRENTRY); 
  lseek(fd, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  write(fd, dirEntries, sizeof(DIRENTRY) * amountOfEntries); 
  return writeBackINODE(workingDir.inodeNum, workingDirINODE); 
}

INODE 
loadINODE(int inodeNum) {
  lseek(fd, getActualAddressOfINODE(inodeNum), SEEK_SET); 
  INODE inode; 
  read(fd, &inode, sizeof(INODE)); 
  return inode; 
}

INODE 
genEmptyINODE() {
  INODE inode; 
  inode.permission = ~0; 
  return inode; 
}

void 
_ls() {
  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  lseek(fd, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  read(fd, dirEntries, sizeof(DIRENTRY) * AMOUNT_OF_DIRENTRY_PER_BLOCK); 
  int amountOfEntries = workingDirINODE.size / sizeof(DIRENTRY); 
  printf("\n# amountOfEntries: %d\n", amountOfEntries); 
  int foo = 0; 
  for (; foo < amountOfEntries; foo++) {
    printf("# %d: %s\n", dirEntries[foo].inodeNum, dirEntries[foo].fileName); 
  }
  printf("\n"); 
}

int 
_mkdir(char *fileName) {
  int inodeNum = findAvailableINODE(); 
  if (-1 == inodeNum) {
    return -1; 
  } else { 
    DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
    dirEntries[0].inodeNum = inodeNum; 
    /*memset(dirEntries[0].fileName, 0, 28); */
    sprintf(dirEntries[0].fileName, "."); 
    dirEntries[1].inodeNum = workingDir.inodeNum; 
    /*memset(dirEntries[1].fileName, 0, 28); */
    sprintf(dirEntries[1].fileName, ".."); 
    INODE inode; 
    inode.permission = getDefaultPermission(1); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 2 * sizeof(DIRENTRY); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    // 将数据写回到datablock中
    lseek(fd, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    write(fd, dirEntries, sizeof(DIRENTRY) * AMOUNT_OF_DIRENTRY_PER_BLOCK); 
    partitionTable->tableDataBlock[inode.headAddress] = 1; 

    writeBackINODE(inodeNum, inode); 

    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    sprintf(newEntry.fileName, "%s", fileName); 
    addDIRENTRY(newEntry); 

    return 0; 
  } 
}

int 
checkEntryExist(char *fileName, 
    DIRENTRY *entry) {
  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  if (-1 == getDirEntriesByINODE(workingDirINODE, dirEntries)) {
    return -1; 
  } else {
    int foo = 0; 
    int amountOfEntries = workingDirINODE.size / sizeof(DIRENTRY); 
    for (; foo < amountOfEntries; foo++) {
      if (1 == compareString(fileName, dirEntries[foo].fileName)) {
        *entry = dirEntries[foo]; 
        return 1; 
      } else {
        continue; 
      }
    }
    return 0; 
  }
}

int 
getDirEntriesByINODE(INODE inode, 
    DIRENTRY *dirEntries) {
  lseek(fd, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
  if (0 >= read(fd, dirEntries, sizeof(DIRENTRY) * AMOUNT_OF_DIRENTRY_PER_BLOCK)) {
    return -1; 
  } else {
    return 0; 
  }
}

int 
_cd(char *fileName) {
  DIRENTRY entry; 
  if (1 != checkEntryExist(fileName, &entry)) {
    sprintf(EMSG, "%s不存在", fileName); 
    return -1; 
  } else {
    workingDir = entry; 
    workingDirINODE = loadINODE(workingDir.inodeNum); 
    if (workingDirINODE.permission & (1 << 9)) {
      printDIRENTRY(workingDir); 
      printINODE(workingDirINODE); 
      println(); 
      return 0; 
    } else {
      sprintf(EMSG, "%s不是目录", fileName); 
      return -1; 
    }
  }
}

void 
printINODE(INODE inode) {
  printf("permission: %o; ", inode.permission); 
  printf("headAddress: %-4d; ", inode.headAddress); 
  printf("size: %-3d", inode.size);  
}

void 
println() {
  printf("\n"); 
}

void 
printDIRENTRY(DIRENTRY entry) {
  printf("%-28s; ", entry.fileName); 
  printf("#%-4d; ", entry.inodeNum); 
}

int 
setRootEntry() {
  rootEntry.inodeNum = 0; 
  sprintf(rootEntry.fileName, "/"); 
}
