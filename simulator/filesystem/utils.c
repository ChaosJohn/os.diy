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
