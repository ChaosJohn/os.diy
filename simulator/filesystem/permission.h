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
 * 根目录项
 * */
extern DIRENTRY rootEntry; 

/* 
 * 检测群组是否存在
 * */
int 
checkGroupExist(char *); 


#endif   /* ----- #ifndef permission_INC  ----- */
