/*
 * =====================================================================================
 *
 *       Filename:  permission.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/15/15 21:30:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  permission_INC
#define  permission_INC

#include  "types.h" 
#include  <stdio.h> 
#include  <unistd.h> 
#include  <fcntl.h> 
#include  <stdlib.h> 


/* 
 * 错误信息
 * */
extern char EMSG[]; 

/* 
 * 当前的用户
 * */
extern USER currentUser; 

/* 
 * 当前用户所在的主群组
 * */
extern GROUP currentGroup; 

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
 * 内存中的当前目录
 * */
extern DIRENTRY workingDir; 
extern INODE workingDirINODE; 

/* 
 * 根目录项
 * */
extern DIRENTRY rootEntry; 

/* 
 * 检测群组是否存在
 * */
int 
checkGroupExist(char *); 


#endif   /* ----- #ifndef permission_INC  ----- */
