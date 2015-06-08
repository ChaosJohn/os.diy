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

#define strFailure "FAILURE" 
#define strSuccess "SUCCESS" 
//char *strFailure = "FAILURE"; 
//char *strSuccess = "SUCCESS"; 

/* 
 * 文件名最大长度
 * */
#define MAX_LENGTH_OF_FILENAME 28 

/* 
 * 用户名和群组名的最大长度
 * */
#define MAX_LENGTH_OF_UGNAME 15 

/* 
 * 密码长度上限
 * */
#define MAX_LENGTH_OF_PASSWORD 10 

/* 
 * 每个用户能加入的群组数量上限
 * */
#define MAX_AMOUNT_OF_GROUPS_PER_USER 6 

/* 
 * 路径深度
 * */
#define PATH_DEPTH 10 

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
 * 每个block能存放的USER个数
 * */
#define AMOUNT_OF_USER_PER_BLOCK (BLOCKSIZE / sizeof(USER)) 
/* 
 * 每个block能存放的GROUP个数
 * */
#define AMOUNT_OF_GROUP_PER_BLOCK (BLOCKSIZE / sizeof(GROUP)) 
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
 * 所有INODE占据的block数量
 * */
#define AMOUNT_OF_DATABLOCK_FOR_INODE (AMOUNT_OF_INODE / (BLOCKSIZE / sizeof(INODE)))
/* 
 * 所有INODE占据的空间大小
 * */
#define SPACE_OF_ALL_INODE (sizeof(INODE) * AMOUNT_OF_INODE)  


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
  char fileName[MAX_LENGTH_OF_FILENAME]; 
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

/* 
 * GROUP数据结构
 * 大小=16
 * */
typedef struct {
  // 群组名 
  char name[MAX_LENGTH_OF_UGNAME]; 
  char gid; 
} GROUP; 

/* 
 * USER数据结构
 * 大小=32 
 * */
typedef struct {
  // 用户名 长度=15
  char name[MAX_LENGTH_OF_UGNAME]; 
  // 用户ID 
  char uid; 
  // 用户所属群组ID 长度=6 
  char gids[MAX_AMOUNT_OF_GROUPS_PER_USER]; 
  char password[MAX_LENGTH_OF_PASSWORD]; 
} USER; 

/* 
 * 命令
 * */
typedef struct {
  char command[32]; 
  char description[64]; 
  int (*execute)(int, char **); 
} CMD; 

/* 
 * 内存空闲表项
 * */
//typedef struct _FreeNode {
  //ADDRESS addr; 
  //int size; 
  //struct _FreeNode *next; 
//}



/* 
 * 缓冲区
 * 大小1024*/
extern char buf[]; 

/* 
 * 错误信息
 * */
extern char EMSG[]; 

/* 
 * root用户和群组
 * */
extern USER rootUser; 
extern GROUP rootGroup; 

/* 
 * 当前的用户
 * */
extern USER currentUser; 

/* 
 * 当前用户所在的主群组
 * */
extern GROUP currentGroup; 

/* 
 * 所有用户
 * */
extern USER users[]; 

/* 
 * 所有群组
 * */
extern GROUP groups[]; 

/* 
 * 用户所在的所有群组
 * */
extern GROUP theGroups[]; 

/* 
 * 虚拟文件系统文件路径
 * */
extern char *vfsPath; 
extern int fd; 

/* 
 * 内存中的分区表
 * */
extern PartitionTable *partitionTable; 

/* 
 * 根目录项
 * */
extern DIRENTRY rootEntry; 

/* 
 * 内存中的当前目录
 * */
extern DIRENTRY workingDir; 
extern INODE workingDirINODE; 

/* 
 * 路径分割后的字符串数组 
 * */
extern char **pathArray; 

/* 
 * 命令分割后的字符串数组
 * */
extern char **cmdArray; 

/* 
 * 命令数组
 * */
extern CMD cmds[]; 
#endif   /* ----- #ifndef types_INC  ----- */
