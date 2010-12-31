#include "terminal.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "uart.h"


/*
 * Use Serial1 as terminal port
 */

#ifdef CHIBI
#include <ch.h>
#include <hal.h>
#define MTX_LOCK() chMtxLock(&mtxTerminalLock)
#define MTX_UNLOCK() chMtxUnlock()
#else
#define MTX_LOCK()
#define MTX_UNLOCK()
#endif

static const char cszHello[] = EOLN
    "Hello happy tester!" EOLN
    "Build date: " __DATE__ " " __TIME__ EOLN EOLN;
static const char cszPrompt[] = "> ";

static int tEcho;

static void _tPuts(const char* str)
{
    uPuts(str);
}

static void _tPutLn(void)
{
    uPuts(EOLN);
}

static void _tPutc(char c)
{
    uPutchar(c);
}

void tPutc(char c)
{
    MTX_LOCK();
    _tPutc(c);
    MTX_UNLOCK();
}

void tPuts(const char* str)
{
    MTX_LOCK();
    _tPuts(str);
    MTX_UNLOCK();
}

void tPutsLn(const char* str)
{
    MTX_LOCK();
    _tPuts(str);
    _tPutLn();
    MTX_UNLOCK();
}

static void _tPutui(uint32_t val)
{
    uint32_t ipow;
    uint32_t power;
    char atmp;
    char prefix_flag = 0;
    for(ipow=0; ipow<(sizeof(power10)/sizeof(power10[0])); ipow++)
    {
        atmp = '0';
        power = power10[ipow];
        while(val >= power)
        {
            atmp++;
            val -= power;
        }
        if(atmp > '0')
            prefix_flag = -1;
        if(prefix_flag)
            uPutchar(atmp);
    }
    uPutchar(val+'0');
}

void tPutui(uint32_t val)
{
    MTX_LOCK();
    _tPutui(val);
    MTX_UNLOCK();
}

static void _tPuti(int32_t val)
{
    if(val<0)
    {
        uPutchar('-');
        val = -val;
    }
    _tPutui(val);
}

void tPuti(int32_t val)
{
    MTX_LOCK();
    _tPuti(val);
    MTX_UNLOCK();
}

static char _encHex(uint8_t v)
{
    if(v<10)
        return '0'+v;
    else
        return 'A'+v-10;
}

static void _tPutHex(uint32_t val, int digits)
{
    if(digits >= 8) uPutchar(_encHex((val>>28) & 0x0F));
    if(digits >= 7) uPutchar(_encHex((val>>24) & 0x0F));
    if(digits >= 6) uPutchar(_encHex((val>>20) & 0x0F));
    if(digits >= 5) uPutchar(_encHex((val>>16) & 0x0F));
    if(digits >= 4) uPutchar(_encHex((val>>12) & 0x0F));
    if(digits >= 3) uPutchar(_encHex((val>>8) & 0x0F));
    if(digits >= 2) uPutchar(_encHex((val>>4) & 0x0F));
    uPutchar(_encHex((val>>0) & 0x0F));
}


void tPutHex(uint32_t val, int digits)
{
    MTX_LOCK();
    _tPutHex(val, digits);
    MTX_UNLOCK();
}

static char szBufBackup[TERM_INPUT_BUFFER];
static char szBufInput[TERM_INPUT_BUFFER];
static int idx_ln;

static int tGetLnStep(void)
{
    int c;

    c = uGetchar();
    if(c <= 0)
        return TK_SKIP;

    if((c == '\b') || (c == 0x7F)) // Backspace
    {
        if(idx_ln>0)
        {
            szBufInput[--idx_ln] = 0;
            if(tEcho)
                tPuts("\b \b");
        }
        else
            if(tEcho) tPutc('\a'); // Bell
    }
    else if((c == '\n') || (c == '\r')) // Enter
    {
        if(szBufInput[0] != 0)
            strcpy(szBufBackup, szBufInput);
        if(tEcho)
            tPuts(EOLN);
        return TK_ENTER;
    }
    else if(c == 0x03) // Cancel line
    {
        tPuts(EOLN);
        idx_ln = 0;
        szBufInput[0] = 0;
        return TK_CANCEL;
    }
    else if(c == 0x10) // Copy from szBufBackup
    {
        if(tEcho && (szBufBackup[0] != 0))
        {
            strcpy(szBufInput, szBufBackup);
            idx_ln = strlen(szBufInput);
            MTX_LOCK();
            _tPutLn();
            _tPuts(cszPrompt);
            _tPuts(szBufInput);
            MTX_UNLOCK();
        }
    }
    else if(c == 0x01) // Clean to home
    {
        if(tEcho)
        {
            MTX_LOCK();
            while(idx_ln--) _tPuts("\b \b");
            MTX_UNLOCK();
            idx_ln = 0;
            szBufInput[0] = 0;
        }
    }
    else if(c >= ' ')
    {
        if(idx_ln < TERM_INPUT_BUFFER-1)
        {
            szBufInput[idx_ln++] = c;
            szBufInput[idx_ln] = 0;
            if(tEcho) tPutc(c);
        }
        else
            if(tEcho) tPutc('\a'); // Bell
    }
    else
    {
        if(tEcho)
        {
            MTX_LOCK();
            _tPutc('\r');
            _tPuts("Debug key: 0x");
            _tPutHex(c, 4);
            _tPutLn();
            _tPuts(cszPrompt);
            _tPuts(szBufInput);
            MTX_UNLOCK();
        }
    }

    return c;
}

typedef struct {
    int reset:1;
    
} tFlags_t;

tFlags_t tFlags;

int tProcess(void)
{
    if(tFlags.reset)
    {
        idx_ln = 0;
        szBufInput[0] = 0;
        tFlags.reset = 0;
        tPuts(cszPrompt);
    }

    int key = tGetLnStep();
    if((key == TK_ENTER) && (idx_ln > 0))
    {
        tFlags.reset = -1;
        char* errorMsg = 0;
        int response = tParse(szBufInput, &errorMsg);
        if(response)
        {
            MTX_LOCK();
            _tPutui(response);
            _tPutc(' ');
            if(errorMsg)
                _tPuts(errorMsg);
            else
            {
                switch(response)
                {
                    case 200: _tPuts("OK"); break;
                    case 400: _tPuts("COMMAND NOT FOUND"); break;
                    case 500: _tPuts("INTERNAL ERROR"); break;
                }
            }
            _tPutLn();
            MTX_UNLOCK();
        }
    }
    return key;
}


void tInit(int echo)
{
	uart0Init(UART_BAUD(BAUD), UART_8N1, UART_FIFO_8); // setup the UART
    tEcho = echo;
    idx_ln = 0;
    szBufInput[0] = 0;
    szBufBackup[0] = 0;
    tFlags.reset = -1;
}


