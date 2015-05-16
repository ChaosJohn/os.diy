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
}

int 
splitPath(char ***pathArrayPointer, 
    char *path) {
  int foo; 
  // 初始化路径数组 
  *pathArrayPointer = (char **) malloc(sizeof(char *) * PATH_DEPTH); 
  char **pathArray = *pathArrayPointer; 
  for (foo = 0; foo < PATH_DEPTH; foo++) {
    *(pathArray + foo) = (char *) malloc(sizeof(char *) * MAX_LENGTH_OF_FILENAME); 
  }
  
  // 分割路径 
  int pathLength = strlen(path); 
  char *cursor = path; 
  char *innerCursor = *pathArray; 
  int arrayIndex = 0; 
  while ('\0' != *cursor) {
    if ('/' == *cursor) {
      if (cursor = path) {
        sprintf(*pathArray, "/"); 
      } else {
        *innerCursor = '\0'; 
      } 
      if ('\0' == *(cursor + 1)) {
        break; 
      }
      arrayIndex++; 
      innerCursor = *(pathArray + arrayIndex); 
    } else {
      *innerCursor = *cursor; 
      innerCursor++; 
    }
    cursor++; 
  }
  return (arrayIndex + 1); 
}
