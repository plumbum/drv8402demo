#ifndef _THERMINAL_H_
#define _THERMINAL_H_

#include <inttypes.h>
#include "config.h"

#ifndef EOLN
#define EOLN "\r\n"
#endif

/**
 * @file terminal.h
 * @brief Debug/configure command parser
 * @details
 *      
 * @addtogroup interface
 * @{
 */

#if (FOSC == 14745600UL)
#define BAUD 115200
#elif (FOSC == 12000000UL)
#define BAUD 38400
#else
#error No BAUD defined
#endif

#define TERM_INPUT_BUFFER 128

#define TK_SKIP     -1
#define TK_ENTER    -2
#define TK_CANCEL   -3

#define uPuts(s) uart0Puts(s)
#define uPutchar(c) uart0Putch(c)
/* Return -1 if no char */
#define uGetchar() uart0Getch()

/**
 * Init serial port hardware and run terminal thread
 */
void tInit(int echo);

void tPutc(char c);

/**
 * Put string direct to terminal
 * @param[in] str - output string
 */
void tPuts(const char* str);

/**
 * Put string direct to terminal. EOLN terminated
 * @param[in] str - output string
 */
void tPutsLn(const char* str);

/**
 * Direct put hex number to terminal
 */
void tPutHex(uint32_t val, int digits);

/**
 * Direct put unsigned integer decimal number to terminal
 */
void tPutui(uint32_t val);

/**
 * Direct put signed integer decimal number to terminal
 */
void tPuti(int32_t val);

/**
 * Callback for new line parse
 */
int tParse(char* ibuf, char** errorMsg);

/**
 * Processing single step
 */
int tProcess(void);

/**
 * @}
 */

#endif /* _THERMINAL_H_ */

