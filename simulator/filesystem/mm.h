/*
 * =====================================================================================
 *
 *       Filename:  mm.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/20/15 14:27:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  mm_INC
#define  mm_INC

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

#define CAPACITY 10000 

/* 
 * 表项
 * */
typedef struct _Block {
  int num; 
  int start; 
  int size; 
  struct _Block *next; 
  struct _Block *prev; 
} BlockNode, *BlockList, *BlockPointer; 

extern BlockList emptyTable; 
extern BlockList allocatedTable; 
extern BlockPointer cursor; 
extern BlockPointer cursorPrev; 
extern BlockPointer nfCursor; 
extern int emptyBlockSize; 
extern int allocatedBlockSize; 
extern int globalIndex; 

void 
initTables(); 

void 
sortTable(BlockList); 

void 
addAllocation(BlockPointer); 

void 
ffMalloc(int); 

void 
bfMalloc(int); 

void 
wfMalloc(int); 

void 
nfMalloc(int); 

void 
_printHorizontalBar(int); 

void 
displayEmptyTable(); 

void 
displayAllocatedTable(); 

void 
mergeEmptyTable(); 

void 
myFree(int); 

//void 
//printCmds(); 


#endif   /* ----- #ifndef mm_INC  ----- */
