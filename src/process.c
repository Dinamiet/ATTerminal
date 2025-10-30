#include "atterminal.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void handle_command(ATTerminal* at, char* command);

static void handle_command(ATTerminal* at, char* response)
{
	(void)at; // Unused

	char* params = NULL;
	if (*response == '+')
	{
		params = strchr(response, ' ');
		if (params)
			*params++ = '\0';
	}

	size_t length = strlen(response);
	if (length)
	{
		printf("\t%s -> '%s'\n", response, params);
	}
}

void ATTerminal_Process(ATTerminal* at)
{
	if (at->Index >= sizeof(at->Unprocessed))
	{
		at->Index -= 1;
		memmove(at->Unprocessed, at->Unprocessed + 1, at->Index);
		memset(&at->Unprocessed[at->Index], 0, sizeof(at->Unprocessed) - at->Index);
	}

	size_t numRead = at->Read(&at->Unprocessed[at->Index], sizeof(at->Unprocessed) - at->Index);
	if (!numRead)
		return;

	at->Index += numRead;

	while (at->Index)
	{
		char* cmdStart = at->Unprocessed;

		while (isspace(*cmdStart))
		{
			if (++cmdStart > &at->Unprocessed[at->Index])
				return;
		}

		char* cmdEnd       = cmdStart;
		char* cmdTerminate = cmdStart;
		while (*cmdTerminate != '\n')
		{
			if (!isspace(*cmdTerminate))
				cmdEnd = cmdTerminate;

			if (++cmdTerminate > &at->Unprocessed[at->Index])
				return;
		}

		*(cmdEnd + 1) = '\0';
		handle_command(at, cmdStart);

		at->Index = &at->Unprocessed[at->Index] - (cmdTerminate + 1);
		memmove(at->Unprocessed, cmdTerminate + 1, at->Index);
		memset(&at->Unprocessed[at->Index], 0, sizeof(at->Unprocessed) - at->Index);
	}
}
