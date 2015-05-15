/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/15 21:35:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  utils_INC
#define  utils_INC

#include  <string.h> 
#include  <stdio.h> 

extern char EMSG[]; 

/* 
 * 比较两个字符串是否一样
 * 相同为0， 不相同为-1
 * */
int 
compareString(char *, 
    char *); 

/* 
 * 打印错误信息
 * */
void 
printEMSG(); 

#endif   /* ----- #ifndef utils_INC  ----- */
