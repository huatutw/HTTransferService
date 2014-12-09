#ifndef _COMMON_H_
#define _COMMON_H_

#define FIRST_S4_INDEX 0

void hexprint(FILE *fp, void *data, int len);
unsigned long OpenS4ByIndex(unsigned long index,PSENSE4_CONTEXT pstS4Ctx);
void ResetAndCloseS4(PSENSE4_CONTEXT pstS4Ctx);

#endif _COMMON_H_
