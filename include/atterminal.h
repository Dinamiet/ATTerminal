#ifndef _ATTERMINAL_H_
#define _ATTERMINAL_H_

#include <stddef.h>

#define MAX_LINE_LENGTH 128

typedef struct _ATTerminal_ ATTerminal;

typedef size_t (*ATTerminal_Interface)(void* data, size_t size);
typedef void (*ATTerminal_NotifierInterface)(ATTerminal* at, char* param);

typedef struct _ATTerminalResponseNotifier_
{
	size_t                       ID;
	ATTerminal_NotifierInterface Notifier_Handler;
} ATTerminalResponseNotifier;

typedef struct _ATTerminal_
{
	ATTerminal_Interface        Read_Handler;
	ATTerminal_Interface        Write_Handler;
	ATTerminalResponseNotifier* NotifierList;
	size_t                      Index;
	char                        Unprocessed[MAX_LINE_LENGTH];
} ATTerminal;

// NOTE: responseNotifiers -> special case for ID of zero
void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* responseNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler);
void ATTerminal_Process(ATTerminal* at);
void ATTerminal_SendCommand(ATTerminal* at, char* command);
void ATTerminal_Wait(ATTerminal* at, char* seq);

#endif
