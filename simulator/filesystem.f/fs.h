/*
 * =====================================================================================
 *
 *       Filename:  fs.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/06/15 19:26:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  fs_INC
#define  fs_INC

#include  "types.h" 

#define FULL_PERMISSION (0777)
#define UMASK_OF_FILE (0133) 
#define UMASK_OF_DIR (0022) 

/* 
 * 虚拟文件系统文件路径
 * */
extern char *vfsPath; 
extern FILE *fp; 

/* 
 * 内存中的分区表
 * */
extern PartitionTable *partitionTable; 

/* 
 * 内存中的当前目录
 * */
extern DIRENTRY workingDir; 
extern INODE workingDirINODE; 

/* 
 * 格式化文件系统
 * */
void 
formatFS(); 

/* 
 * 加载分区表
 * */
PartitionTable * 
loadPartitionTable(); 

/* 
 * 回写分区表
 * */
int 
restorePartitionTable(); 

/* 
 * 创建根目录
 * */
int 
createRootDirectory(); 

/* 
 * 从INODE空闲表中寻找一个空闲项
 * */
int 
findAvailableINODE(); 

/* 
 * 从datablock空闲表中寻找一个空闲项
 * */
int 
findAvailableDataBlock(); 

/* 
 * 取默认权限
 * 根据UMASK计算
 * 传入参数type：1-目录；0-文件*/
int 
getDefaultPermission(int); 

/* 
 * 回写INODE
 * */
int 
writeBackINODE(int, 
    INODE); 

/* 
 * 得到INODE的在虚拟fs中的真实地址
 * */
ADDRESS 
getActualAddressOfINODE(int); 

/* 
 * 打开虚拟fs以供write
 * */
FILE * 
openFSForWrite(); 

/* 
 * 打开虚拟fs以供read
 * */
FILE *
openFSForRead(); 

/* 
 * 创建passwd文件
 * */
int 
createPasswdFile(); 

/* 
 * 创建grp文件
 * */
int 
createGrpFile(); 

/* 
 * 得到DataBlock在虚拟fs中的真实地址
 * */
ADDRESS 
getActualAddressOfDataBlock(int); 

/* 
 * 在当前目录新增目录项
 * */
int 
addDIRENTRY(DIRENTRY); 

/* 
 * 根据inodeNum取得INODE
 * */
INODE 
loadINODE(int); 

/* 
 * 创建空的INODE
 * */
INODE 
genEmptyINODE(); 

/* 
 * ls
 * */
void 
_ls(); 

/* 
 * 创建目录
 * */
int 
_mkdir(char *); 


#endif   /* ----- #ifndef fs_INC  ----- */
