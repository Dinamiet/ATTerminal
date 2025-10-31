#include "atterminal.h"

#include <string.h>

#define COMMAND_END "\r\n"

void ATTerminal_SendCommand(ATTerminal* at, char* command)
{
	at->Write_Handler(command, strlen(command));
	at->Write_Handler(COMMAND_END, strlen(COMMAND_END));
}
