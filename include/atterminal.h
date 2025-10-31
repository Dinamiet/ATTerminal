#ifndef _ATTERMINAL_H_
#define _ATTERMINAL_H_

#include "bufferedlist.h"

#include <stddef.h>

#define MAX_LINE_LENGTH 64

typedef struct _ATTerminal_ ATTerminal;

typedef size_t (*ATTerminal_Interface)(void* data, size_t size);
typedef void (*ATTerminal_NotifierInterface)(ATTerminal* at, char* param);

typedef enum _ATTerminalResponseType_
{
	ATTERMINAL_RESPONSE_ONCE,
	ATTERMINAL_RESPONSE_PERSISTENT,
} ATTerminalResponseType;

typedef struct _ATTerminalResponseNotifier_
{
	size_t                       ResponseID;
	ATTerminalResponseType       Type;
	ATTerminal_NotifierInterface Notifier_Handler;
} ATTerminalResponseNotifier;

typedef struct _ATTerminal_
{
	ATTerminal_Interface Read_Handler;
	ATTerminal_Interface Write_Handler;
	BufferedList         NotifierList;
	size_t               Index;
	char                 Unprocessed[MAX_LINE_LENGTH];
} ATTerminal;

void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* notifierBuffer, size_t numNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler);
void ATTerminal_Process(ATTerminal* at);
void ATTerminal_SendCommand(ATTerminal* at, char* command);

#endif
