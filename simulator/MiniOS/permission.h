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
#include  "fs.h" 
#include  <stdio.h> 
#include  <unistd.h> 
#include  <fcntl.h> 
#include  <stdlib.h> 

#define FULL_PERMISSION (0777)
#define UMASK_OF_FILE (0133) 
#define UMASK_OF_DIR (0022) 

#define UMASK_OF_TYPE (01000) 
#define UMASK_OF_UR (00400) 
#define UMASK_OF_UW (00200) 
#define UMASK_OF_UX (00100) 
#define UMASK_OF_GR (00040) 
#define UMASK_OF_GW (00020) 
#define UMASK_OF_GX (00010) 
#define UMASK_OF_OR (00004) 
#define UMASK_OF_OW (00002) 
#define UMASK_OF_OX (00001) 

/* 
 * 根目录项
 * */
extern DIRENTRY rootEntry; 

/* 
 * 检测群组是否存在
 * */
int 
checkGroupExistByName(char *, 
    GROUP *); 
int 
checkGroupExistByGid(char, 
    GROUP *); 

/* 
 * 检测用户是否存在
 * */
int 
checkUserExistByName(char *, 
    USER *); 
int 
checkUserExistByUid(char, 
    USER *); 

/* 
 * 加载群组文件
 * */
int 
loadGrpFile(int *); 

/* 
 * 加载密码文件
 * */
int 
loadPasswdFile(int *);  

/* 
 * 显示所有群组
 * */
int 
showAllGroups(); 

/* 
 * 显示所有用户
 * */
int 
showAllUsers(); 

/* 
 * 删除群组
 * */
int 
deleteGroupByName(char *); 
int 
deleteGroupByGid(char); 

/* 
 * 删除用户
 * */
int 
deleteUserByName(char *); 
int 
deleteUserByUid(char); 

/* 
 * 增加群组
 * */
int 
addGroup(char *); 

/* 
 * 增加用户，并且指定默认群组
 * */
int 
addUserWithDefaultGroup(char *, 
    char *); 

/* 
 * 加入指定群组
 * */
int 
joinGroup(char *, 
    char *); 

/* 
 * 设置INODE的uid
 * */
void 
setUid(INODE *, 
    char); 

/* 
 * 得到INODE的uid
 * */
char  
getUid(INODE); 

/* 
 * 更改文件的所有者
 * */
int 
_chown(char *, 
    char *); 

/* 
 * 得到文件的uid
 * */
char 
_getUid(char *); 

/* 
 * 设置INODE的gid
 * */
void 
setGid(INODE *, 
    char); 

/* 
 * 得到INODE的gid
 * */
char  
getGid(INODE); 

/* 
 * 更改文件的所属群组
 * */
int 
_chgrp(char *, 
    char *); 

/* 
 * 得到文件的gid
 * */
char 
_getGid(char *); 

/* 
 * 切换用户
 * */
int 
_su(char *); 

/* 
 * 查看当前用户是谁
 * */
void 
_whoami(); 

/* 
 * 修改用户密码
 * */
int 
_passwd(char *); 

/* 
 * 更新用户
 * */
int 
_updateUser(USER); 

/* 
 * 获得指定用户的所有群组
 * */
int 
getTheGroups(char *, 
    int *); 

/* 
 * 打印指定用户的所有群组
 * */
int 
showGroups(char *); 

#endif   /* ----- #ifndef permission_INC  ----- */
