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

/* 
 * 虚拟文件系统文件路径
 * */
extern char *vfsPath; 

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

#endif   /* ----- #ifndef fs_INC  ----- */
