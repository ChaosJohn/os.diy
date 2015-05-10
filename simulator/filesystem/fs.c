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
      fwrite(buf, 1, BLOCKSIZE, fp); 
    }
    printf("格式化完成!\n"); 

    fseek(fp, SEEK_SET, 0); 

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

    fclose(fp); 
  } 
}

PartitionTable * 
loadPartitionTable() {
  FILE *fp; 
  if (NULL != (fp = fopen(vfsPath, "r"))) {
    PartitionTable *partitionTable = (PartitionTable *)malloc(sizeof(PartitionTable)); 
    fread(partitionTable->tableINODE, sizeof(char), AMOUNT_OF_INODE, fp); 
    fread(partitionTable->tableDataBlock, sizeof(char), AMOUNT_OF_DATABLOCK, fp); 
    return partitionTable; 
  } else {
    return NULL; 
  }
}
