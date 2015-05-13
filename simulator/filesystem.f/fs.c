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

void 
formatFS() {
  /*extern char *fileName; */
  FILE *fp; 
  if (NULL != (fp = fopen(vfsPath, "w"))) {
    /*fseek(fp, SEEK_SET, AMOUNT_OF_ALLBLOCK * BLOCKSIZE + 1); */
    /*char buf[] = {EOF}; */
    /*fwrite(buf,1, 1, fp); */
    char buf[BLOCKSIZE] = {0}; 
    printf("格式化...\n"); 
    int foo = 0; 
    // 将整个分区置0 
    for (; foo < AMOUNT_OF_ALLBLOCK; foo++) {
      fwrite(buf, BLOCKSIZE, 1, fp); 
      /*fwrite(buf, 1, BLOCKSIZE, fp); */
    }
    printf("格式化完成!\n"); 

    fseek(fp, 0, SEEK_SET); 

    char availableFlag = 0; 

    // 初始化INODE空闲表
    printf("初始化INODE空闲表...\n"); 
    for (foo = 0; foo < AMOUNT_OF_INODE; foo++) {
      fwrite(&availableFlag, 1, 1, fp); 
    }
    printf("初始化INODE空闲表完成!\n"); 

    // 初始化数据block空闲表 
    printf("初始化数据block空闲表...\n"); 
    for (foo = 0; foo < AMOUNT_OF_DATABLOCK; foo++) {
      fwrite(&availableFlag, 1, 1, fp); 
    }
    printf("初始化数据block空闲表完成!\n"); 

    fflush(fp);  
    fclose(fp); 
  } 
}

PartitionTable * 
loadPartitionTable() {
  PartitionTable *partitionTable = (PartitionTable *)malloc(sizeof(PartitionTable)); 
  fread(partitionTable->tableINODE, sizeof(char), AMOUNT_OF_INODE, fp); 
  fread(partitionTable->tableDataBlock, sizeof(char), AMOUNT_OF_DATABLOCK, fp); 
  fflush(fp);  
  return partitionTable; 
}

int 
restorePartitionTable() {
  fwrite(partitionTable->tableINODE, sizeof(char), AMOUNT_OF_INODE, fp); 
  fwrite(partitionTable->tableDataBlock, sizeof(char), AMOUNT_OF_DATABLOCK, fp); 
  fflush(fp);  
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
    fseek(fp, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    fwrite(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); 
    fflush(fp);  
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
  /*printf("writeBackINODE: %d\n", index); */
  fseek(fp, getActualAddressOfINODE(inodeNum), SEEK_SET); 
  fwrite(&inode, sizeof(INODE), 1, fp); 
  fflush(fp);  
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
    user.gid = 0; 
    user.uid = 0; 
    /*memset(user.name, 0, 13); */
    sprintf(user.name, "root"); 
    /*user.name[14] = 0; */
    INODE inode; 
    inode.permission = getDefaultPermission(0); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 1 * sizeof(USER); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    fseek(fp, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    fwrite(userArray, sizeof(USER), 1, fp); 
    fflush(fp);  
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    // 在当前目录中写入新目录项
    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    /*memset(newEntry.fileName, 0, 28); */
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
    /*memset(group.name, 0, 15); */
    sprintf(group.name, "adljfla"); 
    /*group.name[15] = 0; */
    groupArray[0] = group; 
    INODE inode; 
    inode.permission = getDefaultPermission(0); 
    inode.headAddress = findAvailableDataBlock(); 
    inode.size = 1 * sizeof(GROUP); 
    inode.links = 1; 
    inode.createTime = time(NULL); 
    inode.accessTime = time(NULL); 
    /*rewind(fp); */
    fseek(fp, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    fwrite(groupArray, sizeof(GROUP), 1, fp); 
    /*_ls(); */
    fflush(fp);  
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    // 在当前目录中写入新目录项
    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    /*memset(newEntry.fileName, 0, 28); */
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
    * BLOCKSIZE + SPACE_OF_ALL_INODE + sizeof(BLOCKSIZE) * blockNum; 
  return actualAddress; 
}

int 
addDIRENTRY(DIRENTRY newEntry) {
  DIRENTRY dirEntries[AMOUNT_OF_DIRENTRY_PER_BLOCK]; 
  fseek(fp, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  fread(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); 
  int amountOfEntries = workingDirINODE.size / sizeof(DIRENTRY); 
  /*int foo = 0; */
  /*for (; foo < amountOfEntries; foo++) {*/
    /*printf("%d: %s\n", dirEntries[foo].inodeNum, dirEntries[foo].fileName); */
  /*}*/
  /*printf("amountOfEntries: %d\n", amountOfEntries); */
  dirEntries[amountOfEntries] = newEntry; 
  amountOfEntries++; 
  workingDirINODE.size += sizeof(DIRENTRY); 
  sprintf(dirEntries[0].fileName, "."); 
  fseek(fp, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  /*fwrite(dirEntries, sizeof(DIRENTRY), amountOfEntries + 1, fp); */
  fwrite(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); 
  fflush(fp);  
  return writeBackINODE(workingDir.inodeNum, workingDirINODE); 
}

INODE 
loadINODE(int inodeNum) {
  fseek(fp, getActualAddressOfINODE(inodeNum), SEEK_SET); 
  INODE inode; 
  fread(&inode, sizeof(INODE), 1, fp); 
  fflush(fp);  
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
  fseek(fp, getActualAddressOfDataBlock(workingDirINODE.headAddress), SEEK_SET); 
  fread(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); 
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
    fseek(fp, getActualAddressOfDataBlock(inode.headAddress), SEEK_SET); 
    /*fwrite(dirEntries, sizeof(DIRENTRY), AMOUNT_OF_DIRENTRY_PER_BLOCK, fp); */
    /*fflush(fp);  */
    partitionTable->tableDataBlock[inode.headAddress] = 1; 
    writeBackINODE(inodeNum, inode); 
    DIRENTRY newEntry; 
    newEntry.inodeNum = inodeNum; 
    sprintf(newEntry.fileName, "%s", fileName); 
    addDIRENTRY(newEntry); 
    return 0; 
  } 
}

