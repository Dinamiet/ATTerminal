#include "atterminal.h"

#include <string.h>
#include <stdint.h>

#define MIN(x, y) (x < y ? x : y)

size_t ATTerminal_SendRaw(ATTerminal* at, void* data, size_t size) { return at->Write_Handler(data, size); }

size_t ATTerminal_ReadRaw(ATTerminal* at, void* _data, size_t size)
{
	uint8_t* data= _data;

	size_t length = MIN((size_t)(at->Receive - at->Unused), size);
	memcpy(data, at->Unused, length);

	memmove(at->Unused, at->Unused + length, at->Receive - (at->Unused + length));
	at->Receive -= length;
	memset(at->Receive, 0, length);

	while (length < size)
		length += at->Read_Handler(&data[length], size - length);

	return length;
}
