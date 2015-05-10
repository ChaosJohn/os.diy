/*
 * =====================================================================================
 *
 *       Filename:  types.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/15 20:10:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  types_INC
#define  types_INC

#include <time.h> 
/* 
 * 每个block的大小（字节）
 * */
#define BLOCKSIZE 512 
/* 
 * INODE的数量
 * */
#define AMOUNT_OF_INODE 1024 
/* 
 * 数据block的数量
 * */
#define AMOUNT_OF_DATABLOCK 2048 
/* 
 * 每个block能存放的地址个数
 * 128 
 * */
#define AMOUNT_OF_ADDRESS_PER_BLOCK (BLOCKSIZE / sizeof(ADDRESS)) 
/* 
 * 每个block能存放的目录项个数 
 * 16
 * */
#define AMOUNT_OF_DIRENTRY_PER_BLOCK (BLOCKSIZE / sizeof(DIRENTRY))
/* 
 * INODE空闲表占的block个数
 * */
#define AMOUNT_OF_BLOCK_FOR_TABLE_OF_INODE (sizeof(char) * AMOUNT_OF_INODE / BLOCKSIZE) 
/* 
 * 数据block空闲表占的block个数 
 * */
#define AMOUNT_OF_BLOCK_FOR_TABLE_OF_DATABLOCK (sizeof(char) * AMOUNT_OF_DATABLOCK / BLOCKSIZE) 
/* 
 * 所有block个数
 * 2054
 * */
#define AMOUNT_OF_ALLBLOCK (AMOUNT_OF_BLOCK_FOR_TABLE_OF_INODE + AMOUNT_OF_BLOCK_FOR_TABLE_OF_DATABLOCK + AMOUNT_OF_DATABLOCK) 

/* 
 * 定义地址类型
 * */
typedef int ADDRESS; 

/* 
 * INODE数据结构
 * 大小 = 32 
 * */
typedef struct {
  //char type; // 'd' -> directory; '-' -> file 
  int permission; // 权限
  ADDRESS headAddress;  // 首数据块地址
  //int direct; // 是否是直接寻址，即headAddress指向的数据块存放的是地址还是真实数据
  int size;   // 文件大小, 如果大于1个block，则表示直接寻址 
  int links; // 链接数
  //int dummy[3]; // 为了凑满32个字节 
  time_t createTime; // 创建的时间 
  time_t accessTime; // 最近访问的时间
} INODE; 

/* 
 * 目录项数据结构
 * 大小 = 32
 * */
typedef struct {
  int inodeNum; 
  char fileName[28]; 
} DIRENTRY; 

/* 
 * 分区表数据结构
 * */
typedef struct {
  // INODE空闲表
  char tableINODE[AMOUNT_OF_INODE]; 
  // 数据block空闲表
  char tableDataBlock[AMOUNT_OF_DATABLOCK]; 
} PartitionTable; 

#endif   /* ----- #ifndef types_INC  ----- */
