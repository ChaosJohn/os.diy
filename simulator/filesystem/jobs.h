/*
 * =====================================================================================
 *
 *       Filename:  jobs.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/20/15 16:25:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  jobs_INC
#define  jobs_INC

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include <sys/time.h> 
//#include "utils.c" 

/*
 * 定义三种运行状态
 * */ 
#define RUN 1 
#define WAIT 0 
#define FINISH -1 

/*
 * 定义运行队列中最多能同时工作的作业数
 */
#define MAXJOBSIZE 2 

/*  
 * 作业控制块
 * */
typedef struct _jcb {
  int jid; 
  char *jname; 
  int entertime; 
  int starttime; 
  int needtime; 
  int status; 
  int waittime; 
  int resources[3]; 
  struct _jcb *next; 
} JCBNode, *JCBList, *JCBPointer; 

/* 
 * 运行队列结点数据结构
 * */
typedef struct _RunJob {
  JCBPointer job; 
  struct _RunJob *next; 
} RJNode, *RJList, *RJPointer; 

extern JCBList jcbList; 
extern RJList  runList; 
extern int globalTime; 

/* 
 * 系统中资源总数
 * */
extern int Total[];  

/* 
 * 系统中可用的资源数
 * */
extern int Available[]; 

/* 
 * 输入作业队列
 * */
void 
input(int); 

/* 
 * 打印作业表
 * */
void 
display(); 

/* 
 * 判断是否所有作业都已经完成
 * */
int 
isAllFinished(); 

/* 
 * 更新作业的信息
 * */
void 
updateJobs(); 

/* 
 * 判断系统是否能提供作业所需的资源
 * 能提供 => 返回 1
 * 不满足 => 返回 -1
 * */
int 
isRunnable(JCBPointer); 

/* 
 * 找到需要运行的作业
 * */
JCBPointer 
pickRunning(); 

/* 
 * 初始化系统时间
 * */
void 
initGlobalTime(); 

/* 
 * 找到需要第一个进入队列的作业
 * */
JCBPointer 
pickFirstJob(); 

/* 
 * 使用FCFS调度算法进行作业调度
 * 单道批处理系统
 * */
void 
runFCFS_single(); 

/* 
 * 计算运行队列中有多少作业在运行
 * */
int 
getRunningJobSize(); 

/* 
 * 安排满足条件的作业进入运行队列
 * */
JCBPointer 
insertIntoQueue(); 

/* 
 * 将运行结束的作业从运行队列中移除
 * */
void 
removeFromQueue(); 

/* 
 * 使用FCFS调度算法进行作业调度
 * 多道批处理系统
 * */
void 
runFCFS_multi(); 

/* 
 * 打印一条横线
 * */
void 
printHorizontalBar(); 


/* 
 * 模拟多任务
 * */
int 
scheduleJobs(); 

#endif   /* ----- #ifndef jobs_INC  ----- */
