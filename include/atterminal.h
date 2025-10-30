#ifndef _ATTERMINAL_H_
#define _ATTERMINAL_H_

#include <stddef.h>

#define MAX_LINE_LENGTH 64

typedef size_t (*ATTerminal_Interface)(void* data, size_t size);

typedef struct _ATTerminal_
{
	ATTerminal_Interface Read;
	ATTerminal_Interface Write;
	char                 Unprocessed[MAX_LINE_LENGTH];
	size_t               Index;
} ATTerminal;

void ATTerminal_Init(ATTerminal* at, ATTerminal_Interface read, ATTerminal_Interface write);
void ATTerminal_Process(ATTerminal* at);
void ATTerminal_SendCommand(ATTerminal* at, char* command);

#endif
