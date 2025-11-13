#include "atterminal.h"
#include "fnv.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

static bool read_data(ATTerminal* at);
static void extract_response(ATTerminal* at, char* response);
static void handle_response(ATTerminal* at, char* response, char* params);

static bool read_data(ATTerminal* at)
{
	if (at->Receive - at->Buffer >= MAX_LINE_LENGTH)
	{
		at->Receive -= 1;
		memmove(at->Unused, at->Unused + 1, at->Receive - at->Unused);
		memset(at->Receive, 0, 1);
	}

	size_t numRead = at->Read_Handler(at->Receive, MAX_LINE_LENGTH - (at->Receive - at->Buffer));
	if (!numRead)
		return false;

	at->Receive += numRead;

	return true;
}

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
			if (notifier->Notifier_Handler)
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
	if (!read_data(at))
		return;

	while (at->Receive > at->Buffer)
	{
		char* cmdStart = at->Unused;

		while (isspace(*cmdStart))
		{
			if (++cmdStart > at->Receive)
				return;
		}

		char* cmdEnd       = cmdStart;
		char* cmdTerminate = cmdStart;
		while (*cmdTerminate != '\n' && *cmdTerminate != '\r')
		{
			if (!isspace(*cmdTerminate))
				cmdEnd = cmdTerminate;

			if (++cmdTerminate > at->Receive)
				return;
		}

		*(cmdEnd + 1) = '\0';
		at->Unused    = cmdTerminate + 1;
		extract_response(at, cmdStart);

		memmove(at->Buffer, at->Unused, at->Receive - at->Unused);
		at->Receive -= at->Unused - at->Buffer;
		memset(at->Receive, 0, at->Unused - at->Buffer);
		at->Unused = at->Buffer;
	}
}

void ATTerminal_Wait(ATTerminal* at, char* seq)
{
	char* found = NULL;
	while (!found)
	{
		read_data(at);
		found = strstr(at->Unused, seq);
	}
	if (found)
	{
		size_t length = strlen(seq);
		memmove(found, found + length, at->Receive - (found + length));
		at->Receive -= length;
		memset(at->Receive, 0, length);
	}
}
