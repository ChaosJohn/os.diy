#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include "mm.h" 

#define CAPACITY 10000 

BlockList emptyTable; 
BlockList allocatedTable; 
BlockPointer cursor; 
BlockPointer cursorPrev; 
BlockPointer nfCursor; 
int emptyBlockSize = 1; 
int allocatedBlockSize = 0; 
int globalIndex = 0; 

void 
initTables() {
  allocatedTable = (BlockPointer) malloc(sizeof(BlockNode)); 
  allocatedTable->next = NULL; 
  BlockPointer firstEmptyBlock = (BlockPointer) malloc(sizeof(BlockNode)); 
  firstEmptyBlock->start = 0; 
  firstEmptyBlock->size = CAPACITY; 
  firstEmptyBlock->next = NULL; 
  emptyTable = (BlockPointer) malloc(sizeof(BlockNode)); 
  emptyTable->next = firstEmptyBlock; 
  nfCursor = emptyTable->next; 
}


void 
sortTable(BlockList list) {
  /*BlockPointer head = allocatedTable; */
  BlockPointer head = list; 
  BlockPointer cursor; 
  BlockPointer cursorPrev; 
  BlockPointer maxPrev; 
  BlockPointer max; 
  while(head->next->next != NULL) {
    for (cursor = head->next, max = cursor, cursorPrev = head, maxPrev = cursorPrev; 
        cursor != NULL; 
        cursor = cursor->next, cursorPrev = cursorPrev->next) {
      if (cursor->start < max->start) {
        max = cursor; 
        maxPrev = cursorPrev; 
      }
    }
    maxPrev->next = max->next; 
    max->next = head->next; 
    head->next = max; 
    head = head->next; 
    /*print4test(); */
  }
}

void 
addAllocation(BlockPointer pointer) {
  BlockPointer cursor = allocatedTable->next; 
  BlockPointer cursorPrev = allocatedTable; 
  while (cursor != NULL) {
    cursorPrev = cursor; 
    cursor = cursor->next; 
  }
  cursorPrev->next = pointer; 
  pointer->next = NULL; 
  sortTable(allocatedTable); 
}

void 
ffMalloc(int size) {
  BlockPointer cursor = emptyTable->next; 
  BlockPointer newBlock; 
  while(cursor != NULL) {
    if (size <= cursor->size) {
      newBlock = (BlockPointer) malloc(sizeof(BlockNode)); 
      newBlock->num = globalIndex++; 
      newBlock->start = cursor->start; 
      newBlock->size = size; 
      cursor->size -= size; 
      cursor->start += size; 
      addAllocation(newBlock); 
      printf("分配成功，分区起始地址为 %d ，分配空间为 %d \n", newBlock->start, newBlock->size); 
      return; 
    }
    cursor = cursor->next; 
  }
  printf("没有足够的空间可分配\n"); 
}

void 
bfMalloc(int size) {
  int bestSize = CAPACITY + 1; 
  BlockPointer bestBlock; 
  BlockPointer cursor = emptyTable->next; 
  BlockPointer newBlock; 
  while (cursor != NULL) {
    if (size <= cursor->size && cursor->size < bestSize) {
      bestBlock = cursor; 
      bestSize = cursor->size; 
    } 
    cursor = cursor->next; 
  }
  if (bestSize != CAPACITY + 1) {
    newBlock = (BlockPointer) malloc(sizeof(BlockNode)); 
    newBlock->num = globalIndex++; 
    newBlock->start = bestBlock->start; 
    newBlock->size = size; 
    bestBlock->size -= size; 
    bestBlock->start += size; 
    addAllocation(newBlock); 
    printf("分配成功，分区起始地址为 %d ，分配空间为 %d \n", newBlock->start, newBlock->size); 
  } else {
    printf("没有足够的空间可分配\n"); 
  }
}


void 
wfMalloc(int size) {
  int bestSize = -1; 
  BlockPointer bestBlock; 
  BlockPointer cursor = emptyTable->next; 
  BlockPointer newBlock; 
  while (cursor != NULL) {
    if (size <= cursor->size && cursor->size > bestSize) {
      bestBlock = cursor; 
      bestSize = cursor->size; 
    } 
    cursor = cursor->next; 
  }
  if (bestSize != -1) {
    newBlock = (BlockPointer) malloc(sizeof(BlockNode)); 
    newBlock->num = globalIndex++; 
    newBlock->start = bestBlock->start; 
    newBlock->size = size; 
    bestBlock->size -= size; 
    bestBlock->start += size; 
    addAllocation(newBlock); 
    printf("分配成功，分区起始地址为 %d ，分配空间为 %d \n", newBlock->start, newBlock->size); 
  } else {
    printf("没有足够的空间可分配\n"); 
  }
}


void 
nfMalloc(int size) {
  BlockPointer newBlock; 
  int round = 0; 
  while (round++ < 2) {
    if (cursor == NULL) {
      cursor = emptyTable->next; 
    }
    while(cursor != NULL) {
      if (size <= cursor->size) {
        newBlock = (BlockPointer) malloc(sizeof(BlockNode)); 
        newBlock->num = globalIndex++; 
        newBlock->start = cursor->start; 
        newBlock->size = size; 
        cursor->size -= size; 
        cursor->start += size; 
        addAllocation(newBlock); 
        printf("分配成功，分区起始地址为 %d ，分配空间为 %d \n", newBlock->start, newBlock->size); 
        return; 
      }
      cursor = cursor->next; 
    }
  } 
  printf("没有足够的空间可分配\n"); 
}

void 
_printHorizontalBar(int type) {
  int length = (type == 0) ? 17 : 25; 
  int i = 0; 
  for (; i < length; i++) 
    printf("-"); 
  printf("\n"); 
}

void 
displayEmptyTable() {
  BlockPointer cursor = emptyTable->next; 
  _printHorizontalBar(0); 
  printf("|  空闲分区表  \t|\n"); 
  _printHorizontalBar(0); 
  printf("|地址\t|大小\t|\n"); 
  _printHorizontalBar(0); 
  while (cursor != NULL) {
    printf("|%d\t|%d\t|\n", cursor->start, cursor->size); 
    cursor = cursor->next; 
  }
  _printHorizontalBar(0); 
  printf("\n"); 
}


void 
displayAllocatedTable() {
  BlockPointer cursor = allocatedTable->next; 
  _printHorizontalBar(1); 
  printf("|     分配分区表     \t|\n"); 
  _printHorizontalBar(1); 
  printf("|标识\t|地址\t|大小\t|\n"); 
  _printHorizontalBar(1); 
  while (cursor != NULL) {
    printf("|%d\t|%d\t|%d\t|\n", cursor->num, cursor->start, cursor->size); 
    cursor = cursor->next; 
  }
  _printHorizontalBar(1); 
  printf("\n"); 
}

void 
mergeEmptyTable() {
  BlockPointer cursorPrev = emptyTable->next; 
  BlockPointer cursor = cursorPrev->next; 
  while (cursor != NULL) {
    if (cursorPrev->start + cursorPrev->size == cursor->start) {
      cursorPrev->next = cursor->next; 
      cursorPrev->size += cursor->size; 
      free(cursor); 
      cursor = cursorPrev->next; 
      continue; 
    }
    cursorPrev = cursor; 
    cursor = cursor->next; 
  }
}

void 
myFree(int id) {
  BlockPointer cursorPrev = allocatedTable; 
  BlockPointer cursor = allocatedTable->next; 
  while (cursor != NULL) {
    if (id == cursor->num) {
      cursorPrev->next = cursor->next; 
      cursor->next = emptyTable->next; 
      emptyTable->next = cursor; 
      sortTable(emptyTable); 
      printf("回收成功: 从 %d 处回收了 %d 的空间\n", cursor->start, cursor->size); 
      mergeEmptyTable(); 
      return; 
    }
    cursorPrev = cursor; 
    cursor = cursor->next; 
  }
}

