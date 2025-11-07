#include "atterminal.h"

#include <string.h>

void ATTerminal_Wait(ATTerminal* at, char* seq)
{
	const size_t length = strlen(seq);
	char         buffer[length + 1];

	memset(buffer, '\0', length + 1);

	at->Read_Handler(buffer, length);
	while (strcmp(seq, buffer) != 0)
	{
		memmove(buffer, buffer + 1, length - 1);
		at->Read_Handler(&buffer[length - 1], 1);
	}
}
