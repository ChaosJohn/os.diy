/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/15 22:08:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chaos John (CJ), chaosjohn.yjh@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "utils.h" 

int 
compareString(char *strA, 
    char *strB) {
  if (strlen(strA) != strlen(strB)) {
    return 0; 
  }
  char *cursorA = strA; 
  char *cursorB = strB; 
  while (*cursorA != '\0' && *cursorB != '\0') {
    if (*cursorA == *cursorB) {
      cursorA++; 
      cursorB++; 
      continue; 
    } else {
      return 0; 
    }
  }
  return 1; 
}

void 
printEMSG() {
  printf("%s\n", EMSG); 
  resetEMSG(); 
}

void 
resetEMSG() {
  memset(EMSG, 0, 1024); 
}

int 
splitPath(char *path) {
  /*printf("%s\n", path); */
  char *cursor = path; 
  char *innerCursor = *pathArray; 
  memset(innerCursor, 0, MAX_LENGTH_OF_FILENAME); 
  int arrayIndex = 0; 
  while ('\0' != *cursor) {
    if ('/' == *cursor) {
      if (cursor == path) {
        /*memset(*pathArray, 0, MAX_LENGTH_OF_FILENAME); */
        sprintf(*pathArray, "/"); 
      } else {
        *innerCursor = '\0'; 
      }
      if ('\0' == *(cursor + 1)) {
        break; 
      }
      arrayIndex++; 
      innerCursor = *(pathArray + arrayIndex); 
      memset(innerCursor, 0, MAX_LENGTH_OF_FILENAME); 
    } else {
      *innerCursor = *cursor; 
      innerCursor++; 
    }
    cursor++; 
  }
  return (arrayIndex + 1); 
}

int 
splitCmd(char *cmd) {
  if ('\n' == cmd[strlen(cmd) - 1]) {
    cmd[strlen(cmd) - 1] = '\0'; 
  }
  int arrayIndex = 0; 
  char *cursor = *cmdArray; 
  char pattern[] = " "; 
  char *iter = strtok(cmd, pattern); 
  while (NULL != iter) {
    memset(cursor, 0, 32); 
    sprintf(cursor, "%s", iter); 
    /*printf("%s\n", iter); */
    /*printf("%s\n", cursor); */
    iter = strtok(NULL, pattern); 
    arrayIndex++; 
    cursor = *(cmdArray + arrayIndex); 
  }
  return arrayIndex; 
}

/* 
 * 打印一条横线
 * */
/*void */
/*printHorizontalBar() {*/
  /*int i; */
  /*for (i = 0; i <= 40; i++) */
    /*printf("-"); */
  /*printf("\n"); */
/*} */
