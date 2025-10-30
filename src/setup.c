#include "atterminal.h"

#include <string.h>

void ATTerminal_Init(ATTerminal* at, ATTerminal_Interface read_interface, ATTerminal_Interface write_interface)
{
	at->Read  = read_interface;
	at->Write = write_interface;
	memset(at->Unprocessed, 0, sizeof(at->Unprocessed));
}
