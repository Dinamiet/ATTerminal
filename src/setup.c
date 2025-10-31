#include "atterminal.h"

#include <string.h>

void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* responseNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler)
{
	at->Read_Handler  = read_handler;
	at->Write_Handler = write_handler;

	at->NotifierList = responseNotifiers;

	at->Index = 0;

	memset(at->Unprocessed, 0, sizeof(at->Unprocessed));
}
