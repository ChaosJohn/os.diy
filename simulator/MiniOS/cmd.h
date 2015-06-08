/*
 * =====================================================================================
 *
 *       Filename:  cmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/19/15 15:55:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  cmd_INC
#define  cmd_INC

#include  <stdio.h> 
#include  <unistd.h> 
#include  <stdlib.h> 
#include  <malloc.h> 
#include  "types.h" 
#include  "fs.h" 
#include  "utils.h" 
#include  "permission.h" 
#include  "mm.h" 
#include  "jobs.h" 

/* 
 * 执行指令
 * */
int 
executeCmd(char *); 

/* 
 * 打印help命令
 * */
int 
printCmds(int,  
    char **); 

/* 
 * 退出
 * */
int 
quit(int, 
    char **); 

/* 
 * 创建文件或修正文件时间
 * */
int 
touch(int, 
    char **); 

/* 
 * 创建目录
 * */
int 
mkdir(int, 
    char **); 

/* 
 * 列出当前目录的文件清单
 * */
int 
ls(int, 
    char **); 

/* 
 * 按长格式列出当前目录的文件清单
 * */
int 
ll(int, 
    char **); 

/* 
 * 更改文件的所有者
 * */
int 
chowner(int, 
    char **); 

/* 
 * 更改文件的所属群组
 * */
int 
chgroup(int, 
    char **); 

/* 
 * 新增群组
 * */
int 
groupadd(int, 
    char **); 

/* 
 * 新增用户
 * */
int 
useradd(int, 
    char **); 

/* 
 * 更新用户密码
 * */
int 
passwd(int, 
    char **); 

/* 
 * 切换用户
 * */
int 
su(int, 
    char **); 

/*
 * 查看当前的用户
 */
int 
whoami(int, 
    char **); 

/* 
 * 加入群组
 * */
int 
joingroup(int, 
    char **); 

/* 
 * 显示指定用户的所有群组
 * */
int 
showgroups(int, 
    char **); 

/* 
 * 显示所有用户
 * */
int 
lsusers(int, 
    char **); 

/* 
 * 显示所有群组
 * */
int 
lsgroups(int, 
    char **); 

/* 
 * 更改目录
 * */
int 
cd(int, 
    char **); 

/* 
 * 使用首次适应算法分配内存
 * */
int 
ffmalloc(int, 
    char **); 

/* 
 * 使用循环首次适应算法分配内存
 * */
int 
nfmalloc(int, 
    char **); 

/* 
 * 使用最佳适应算法分配内存
 * */
int 
bfmalloc(int, 
    char **); 

/* 
 * 使用最差适应算法分配内存
 * */
int 
wfmalloc(int, 
    char **); 

/* 
 * 显示内存空闲表和内存分配表
 * */
int 
showmm(int, 
    char **); 

/* 
 * 显示内存空闲表
 * */
int 
showempty(int, 
    char **); 

/* 
 * 显示内存分配表
 * */
int 
showused(int, 
    char **); 

/* 
 * 回收内存
 * */
int 
freemm(int, 
    char **); 

/* 
 * 模拟多任务
 * */
int 
runjobs(int, 
    char **); 

#endif   /* ----- #ifndef cmd_INC  ----- */
