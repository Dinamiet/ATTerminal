#include "atterminal.h"

#include <string.h>

void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* responseNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler, ATTerminal_TimeInterface time_handler)
{
	at->Time_Handler  = time_handler;
	at->Read_Handler  = read_handler;
	at->Write_Handler = write_handler;

	at->NotifierList = responseNotifiers;

	at->Unused  = at->Buffer;
	at->Receive = at->Buffer;

	memset(at->Buffer, 0, sizeof(at->Buffer));
}
