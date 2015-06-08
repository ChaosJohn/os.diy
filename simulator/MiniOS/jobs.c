#include  "jobs.h" 

JCBList jcbList; 
RJList  runList; 
int globalTime; 

/* 
 * 系统中资源总数
 * */
int Total[3] = {10, 10, 10};  

/* 
 * 系统中可用的资源数
 * */
int Available[3] = {10, 10, 10}; 

/* 
 * 输入作业队列
 * */
void 
input(int jcbSize) {
  int i; 
  JCBPointer pointer; 
  jcbList = (JCBPointer)malloc(sizeof(JCBNode)); 
  jcbList->next = NULL; 
  runList = (RJPointer)malloc(sizeof(RJNode)); 
  runList->next = NULL; 
  JCBPointer cursor = jcbList; 
  for (i = 0; i < jcbSize; i++) {
    pointer = (JCBPointer)malloc(sizeof(JCBNode)); 
    printf("建立第  %d  个作业\n", i); 
    pointer->jid = i; 
    printf("\t进入时间: "); scanf("%d", &(pointer->entertime)); fflush(stdin); 
    printf("\t所需时间: "); scanf("%d", &(pointer->needtime)); fflush(stdin); 
    printf("\t所需资源: "); scanf("%d %d %d", &(pointer->resources[0]), 
        &(pointer->resources[1]), &(pointer->resources[2])); 
    pointer->status = WAIT; 
    pointer->waittime = 0; 
    pointer->next = NULL; 
    cursor->next = pointer; 
    cursor = cursor->next; 
  }
}


void 
sortFCFS() {

}

/* 
 * 打印作业表
 * */
void 
display() {
  JCBPointer cursor = jcbList->next; 
  printHorizontalBar(); 
  printf("|作业号\t|状态\t|进入\t|需要\t|等待\t|\n"); 
  printHorizontalBar(); 
  while(cursor != NULL) {
    char status; 
    if (cursor->status == RUN) 
      status = 'R'; 
    else if (cursor->status == WAIT) 
      status = 'W'; 
    else if (cursor->status == FINISH) 
      status = 'F'; 
    printf("|%d\t|%c\t|%d\t|%d\t|%d\t|\n", 
        cursor->jid, status, cursor->entertime, 
        cursor->needtime, cursor->waittime); 
    cursor = cursor->next; 
  }
  printHorizontalBar(); 
}

/* 
 * 判断是否所有作业都已经完成
 * */
int 
isAllFinished() {
  JCBPointer cursor = jcbList->next; 
  while(cursor != NULL) {
    if(cursor->status != FINISH) 
      return -1; 
    cursor = cursor->next; 
  }
  return 1; 
}

/* 
 * 更新作业的信息
 * */
void 
updateJobs() {
  JCBPointer cursor = jcbList->next; 
  while(cursor != NULL) {
    if (cursor->status == WAIT) {
      if (cursor->entertime <= globalTime) {
        cursor->waittime = globalTime - cursor->entertime; 
      }
    }
    cursor = cursor->next; 
  }
}

/* 
 * 判断系统是否能提供作业所需的资源
 * 能提供 => 返回 1
 * 不满足 => 返回 -1
 * */
int 
isRunnable(JCBPointer pointer) {
  int i = 0; 
  for (; i < 3; i++) {
    if (pointer->resources[i] > Available[i]) 
      return -1; 
  }
  return 1; 
}

/* 
 * 找到需要运行的作业
 * */
JCBPointer 
pickRunning() {
  updateJobs(); 
  JCBPointer cursor; 
  JCBPointer running = jcbList->next; 
  while(1) {
    if (running->status != FINISH) 
      break; 
    running = running->next; 
  }
  cursor = running; 
  while(cursor != NULL) {
    if (cursor->status != FINISH) {
      if (cursor->waittime > running->waittime) {
        running = cursor; 
      } 
    }
    cursor = cursor->next; 
  }
  return running; 
}

/* 
 * 初始化系统时间
 * */
void 
initGlobalTime() {
  JCBPointer cursor = jcbList->next; 
  globalTime = cursor->entertime; 
  while(cursor != NULL) {
    if (cursor->entertime < globalTime) {
      globalTime = cursor->entertime; 
    } 
    cursor = cursor->next; 
  }
  cursor = jcbList->next; 
  while(cursor != NULL) {
    cursor->waittime = globalTime; 
    cursor = cursor->next; 
  }
}

/* 
 * 找到需要第一个进入队列的作业
 * */
JCBPointer 
pickFirstJob() {
  JCBPointer cursor = jcbList->next; 
  while(cursor != NULL) {
    if (cursor->entertime == globalTime) 
      return cursor; 
    cursor = cursor->next; 
  }
  return NULL; 
}

/* 
 * 使用FCFS调度算法进行作业调度
 * 单道批处理系统
 * */
void 
runFCFS_single() {
  /*JCBPointer head = jcbList; */
  initGlobalTime(); 
  JCBPointer running; 
  int isFirst = 1; 
  while (isAllFinished() != 1) {
    /*sortFCFS(); */
    /*running = head->next; */
    if (isFirst) {
      running = pickFirstJob(); 
      isFirst = 0; 
    } else {
      running = pickRunning(); 
    } 
    running->starttime = globalTime; 
    running->status = RUN; 
    printf("\n#=>正在运行第  %d  个作业\n", running->jid); 
    display(); 
    sleep(1); 
    running->status = FINISH; 
    printf("#=>第  %d  个作业已经运行结束\n", running->jid); 
    globalTime += running->needtime; 
    sleep(1); 
    updateJobs(); 
  }
}

/* 
 * 计算运行队列中有多少作业在运行
 * */
int 
getRunningJobSize() {
  int size = 0; 
  RJPointer cursor = runList->next; 
  while (cursor != NULL) {
    size++; 
    cursor = cursor->next; 
  }
  return size; 
}

/* 
 * 安排满足条件的作业进入运行队列
 * */
JCBPointer 
insertIntoQueue() {
  if (getRunningJobSize() >= MAXJOBSIZE) 
    return NULL; 
  JCBPointer cursor = jcbList->next; 
  JCBPointer job; 
  while (cursor != NULL) {
    //对所有等待状态的作业进行操作
    if (cursor->status != WAIT) {
      cursor = cursor->next; 
      continue; 
    }
    if (isRunnable(cursor) == 1) 
      break; 
    cursor = cursor->next; 
  }
  if (cursor == NULL) 
    return NULL; 
  job = cursor; 
  while (cursor != NULL) {
    if (cursor->status != WAIT) {
      cursor = cursor->next; 
      continue; 
    }
    if (isRunnable(cursor) == 1 && cursor->entertime < job->entertime) 
      job = cursor; 
    cursor = cursor->next; 
  }
  if (job->entertime > globalTime) 
    return NULL; 
  job->status = RUN; 
  job->starttime = globalTime; 
  printf("第  %d  个作业进入运行状态\n", job->jid); 
  int i = 0; 
  for (; i < 3; i++) 
    Available[i] -= job->resources[i]; 
  //将作业填入运行队列
  RJPointer runjob = (RJPointer)malloc(sizeof(RJNode)); 
  runjob->job = job; 
  runjob->next = runList->next; 
  runList->next = runjob; 
  return job; 
}

/* 
 * 将运行结束的作业从运行队列中移除
 * */
void 
removeFromQueue() {
  RJPointer cursor = runList->next; 
  RJPointer cursorPrev = runList; 
  while (cursor != NULL) {
    if (globalTime - cursor->job->starttime == cursor->job->needtime) {
      int i = 0; 
      for (; i < 3; i++) 
        Available[i] += cursor->job->resources[i]; 
      cursor->job->status = FINISH; 
      printf("第  %d  个作业已经运行结束\n", cursor->job->jid); 
      cursor = cursor->next; 
      cursorPrev->next = cursor; 
      continue; 
    }
    cursorPrev = cursor; 
    cursor = cursor->next; 
  }
}

/* 
 * 使用FCFS调度算法进行作业调度
 * 多道批处理系统
 * */
void 
runFCFS_multi() {
  JCBPointer job; 
  while (isAllFinished() != 1) {
    printf("globalTime: %d\n", globalTime); 
    /*printf("updateJobs\n"); */
    updateJobs(); 
    //安排满足条件的作业进入运行队列 && 打印
    /*printf("insertIntoQueue\n"); */
    if ((job = insertIntoQueue()) != NULL) {
      display(); 
    }
    sleep(1); 
    //将运行结束的作业从运行队列中移除
    /*printf("removeFromQueue\n"); */
    removeFromQueue(); 
    globalTime++; 
  }
}

/* 
 * 打印一条横线
 * */
void 
printHorizontalBar() {
  int i;  
  for (i = 0; i <= 40; i++) 
    printf("-"); 
  printf("\n"); 
} 

/* 
 * 模拟多任务
 * */
int 
scheduleJobs() {
  printf("资源总数: %d %d %d\n", Total[0], Total[1], Total[2]); 
  int jcbSize = 0; 
  /*int choose = 0; */
  printf("需要建立的作业个数: "); scanf("%d", &jcbSize); fflush(stdin); 
  input(jcbSize); 
  display(); 
  runFCFS_multi(); 
  getchar(); 

  printf("\n#=>全部作业已经结束\n"); 

  return 0; 
}
