#include "atterminal.h"

#include <string.h>

#define COMMAND_END       "\r\n"
#define BUSY_WAIT_TIMEOUT 60000

void ATTerminal_SendCommand(ATTerminal* at, char* command)
{
	uint32_t startTime = at->Time_Handler();
	while (at->Busy && (at->Time_Handler() - startTime) < BUSY_WAIT_TIMEOUT) { ATTerminal_Process(at); }
	at->Write_Handler(command, strlen(command));
	at->Write_Handler(COMMAND_END, strlen(COMMAND_END));
	at->Busy = true;
}
