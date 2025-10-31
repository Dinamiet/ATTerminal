#include "atterminal.h"
#include "fnv.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void extract_response(ATTerminal* at, char* response);
static void handle_response(ATTerminal* at, char* response, char* params);

static void extract_response(ATTerminal* at, char* response)
{
	(void)at; // Unused

	char* params = NULL;
	if (*response == '+')
	{
		++response;
		params = strchr(response, ':');
		if (params)
			*params++ = '\0';
		while (isspace(*params)) ++params;
	}

	size_t length = strlen(response);
	if (length)
	{
		handle_response(at, response, params);
	}
}

static void handle_response(ATTerminal* at, char* response, char* params)
{
	ATTerminalResponseNotifier* notifier = at->NotifierList;
	if (!notifier)
		return;

	size_t id = FNV(response, strlen(response));
	while (notifier->ID)
	{
		if (notifier->ID == id)
		{
			notifier->Notifier_Handler(at, params);
			return;
		}
		notifier++;
	}

	if (notifier->Notifier_Handler)
		notifier->Notifier_Handler(at, response);
}

void ATTerminal_Process(ATTerminal* at)
{
	if (at->Index >= sizeof(at->Unprocessed))
	{
		at->Index -= 1;
		memmove(at->Unprocessed, at->Unprocessed + 1, at->Index);
		memset(&at->Unprocessed[at->Index], 0, sizeof(at->Unprocessed) - at->Index);
	}

	size_t numRead = at->Read_Handler(&at->Unprocessed[at->Index], sizeof(at->Unprocessed) - at->Index);
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
		extract_response(at, cmdStart);

		at->Index = &at->Unprocessed[at->Index] - (cmdTerminate + 1);
		memmove(at->Unprocessed, cmdTerminate + 1, at->Index);
		memset(&at->Unprocessed[at->Index], 0, sizeof(at->Unprocessed) - at->Index);
	}
}
