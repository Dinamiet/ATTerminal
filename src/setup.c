#include "atterminal.h"

#include <string.h>

void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* notifierBuffer, size_t numNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler)
{
	at->Read_Handler  = read_handler;
	at->Write_Handler = write_handler;

	BufferedList_Init(&at->NotifierList, notifierBuffer, sizeof(*notifierBuffer), numNotifiers);

	at->Index = 0;

	memset(at->Unprocessed, 0, sizeof(at->Unprocessed));
}
