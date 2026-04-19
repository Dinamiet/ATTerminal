#ifndef _ATTERMINAL_H_
#define _ATTERMINAL_H_

/**
 * \file
 * AT command terminal interface for embedded systems.
 *
 * This module provides a simple interface for sending AT commands and processing responses.
 * It is designed to be flexible and can be used with various communication interfaces (e.g., UART, SPI) by providing appropriate read/write handlers.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 127 /**< Maximum length of a line in the terminal buffer */

typedef struct _ATTerminal_ ATTerminal;

/**
 * ATTerminal Read/Write interface
 * \param data Pointer to the data buffer for reading/writing.
 * \param size Size of the data to read/write.
 * \return Number of bytes read/written.
 */
typedef size_t (*ATTerminal_Interface)(void* data, size_t size);

/**
 * ATTerminal Notifier interface
 * \param at Pointer to the ATTerminal instance.
 * \param param Parameter passed to the notifier, typically the command response.
 */
typedef void (*ATTerminal_NotifierInterface)(ATTerminal* at, char* param);

/**
 * ATTerminal Time interface
 * \return Current time in milliseconds.
 */
typedef uint32_t (*ATTerminal_TimeInterface)(void);

/**
 * ATTerminal Response Notifier structure
 */
typedef struct _ATTerminalResponseNotifier_
{
	size_t                       ID;               /**< Unique identifier for the response */
	ATTerminal_NotifierInterface Notifier_Handler; /**< Notifier handler function */
} ATTerminalResponseNotifier;

/**
 * ATTerminal structure
 */
typedef struct _ATTerminal_
{
	bool                        Busy;
	ATTerminal_TimeInterface    Time_Handler;
	ATTerminal_Interface        Read_Handler;
	ATTerminal_Interface        Write_Handler;
	ATTerminalResponseNotifier* NotifierList;
	char*                       Unused;
	char*                       Receive;
	char                        Buffer[MAX_LINE_LENGTH + 1];
} ATTerminal;

/**
 * Initialize the ATTerminal instance.
 * \param at The ATTerminal instance to initialize.
 * \param responseNotifiers Pointer to an array of ATTerminalResponseNotifier structures, terminated by an entry with ID of zero.
 * \param read_handler Function pointer for the read handler interface.
 * \param write_handler Function pointer for the write handler interface.
 * \param time_handler Function pointer for the time handler interface.
 * \note responseNotifier with ID of zero and a valid callback will be called for any response that does not match any other notifier in the list.
 * This allows for a default handler to be set up for unmatched responses.
 */
void ATTerminal_Init(ATTerminal* at, ATTerminalResponseNotifier* responseNotifiers, ATTerminal_Interface read_handler, ATTerminal_Interface write_handler, ATTerminal_TimeInterface time_handler);

/**
 * Process incoming data and handle complete lines of responses.
 * This function should be called periodically to ensure that incoming data is processed and appropriate notifiers are called for complete lines of responses.
 * \param at The ATTerminal instance to process.
 */
void ATTerminal_Process(ATTerminal* at);

/**
 * Send an AT command to the terminal.
 * \param at The ATTerminal instance to send the command through.
 * \param command The AT command string to send, which should be null-terminated.
 */
void ATTerminal_SendCommand(ATTerminal* at, char* command);

/**
 * Send raw data to the terminal.
 * \param at The ATTerminal instance to send the data through.
 * \param data Pointer to the data buffer to send.
 * \param size Size of the data to send.
 * \return Number of bytes sent.
 */
size_t ATTerminal_SendRaw(ATTerminal* at, void* data, size_t size);

/**
 * Read raw data from the terminal.
 * \param at The ATTerminal instance to read from.
 * \param data Pointer to the buffer to store the read data.
 * \param size Size of the buffer to read into.
 * \return Number of bytes read.
 */
size_t ATTerminal_ReadRaw(ATTerminal* at, void* data, size_t size);

/**
 * Wait for a specific sequence in the terminal's response buffer.
 * \param at The ATTerminal instance to wait on.
 * \param seq The null-terminated string sequence to wait for in the response buffer.
 * \return true if the sequence was found within the timeout period, false otherwise.
 */
bool ATTerminal_Wait(ATTerminal* at, char* seq);

/**
 * Check if the terminal is currently busy processing a command or response.
 * \param at The ATTerminal instance to check.
 * \return true if the terminal is busy, false otherwise.
 */
bool ATTerminal_IsBusy(ATTerminal* at);

/**
 * Force the terminal's busy state.
 * \param at The ATTerminal instance to modify.
 * \param busy The busy state to set (true to force busy, false to clear busy state).
 * \note This function can be used to manually set the terminal's busy state, which may be necessary in certain situations where the terminal's state needs to be controlled externally.
 * Use with caution, as forcing the busy state can interfere with normal operation if not used correctly.
 */
void ATTerminal_ForceBusy(ATTerminal* at, bool busy);

#endif
