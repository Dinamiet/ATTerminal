#include "atterminal.h"

#include <string.h>

#define COMMAND_END "\r\n"

void ATTerminal_SendCommand(ATTerminal* at, char* command)
{
	at->Write(command, strlen(command));
	at->Write(COMMAND_END, strlen(COMMAND_END));
}
