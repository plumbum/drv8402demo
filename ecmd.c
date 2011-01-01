#include "ecmd.h"

//#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef int (ecmdfunc_t)(char*);

/**
 * @brief Command list define
 */
typedef struct {
    const char* command;    /**< Command @p value */
    ecmdfunc_t* func;       /**< Command function handler */
    const char* help;       /**< Short help for command */
} ecmds_t;

const ecmds_t ecmds[] = {
    { "HELP",       ecmdHelp,   "This help" },
    { "STOP",       ecmdStop,   "Stop motor" },
    { "SPEED",       ecmdSpeed,   "Set speed, start motor. <speed_rpm>" },
    // { "TIME",       ecmdTime,   "Read current time" },
    // { "SETIME",     ecmdSetime, "Set new time [YY/MM/DD HH:MM:SS]" },
};

int ecmdStop(char* str)
{
    (void)str;
    return 200;
}

int ecmdSpeed(char* str)
{
    (void)str;
    return 200;
}

int ecmdHelp(char* str)
{
    (void)str;
    unsigned int i;
    tPutsLn("About commans");
    for(i=0; i<(sizeof(ecmds)/sizeof(ecmds[0])); i++)
    {
        tPuts("    ");
        tPuts(ecmds[i].command);
        tPuts("\t");
        tPutsLn(ecmds[i].help);
    }
    return 200;
}

int ecmdTime(char* str)
{
    (void)str;
    tPuts("System time: ");
    /*
    time_t tm = 0; // rtcGetCounter();
    char* pC = ctime(&tm);
    tPutsLn(pC);
    */
    tPuts(EOLN);
    return 200;
}

int ecmdSetime(char* str)
{
    /*
    struct tm stime;
    char* pch = strtok(str, "/");
    if(pch == NULL) return -1;
    stime.tm_year = atoi(pch);

    pch = strtok(NULL, "/");
    if(pch == NULL) return -1;
    stime.tm_mon = atoi(pch)-1;

    pch = strtok(NULL, " ");
    if(pch == NULL) return -1;
    stime.tm_mday = atoi(pch);

    pch = strtok(NULL, ":");
    if(pch == NULL) return -1;
    stime.tm_hour = atoi(pch);

    pch = strtok(NULL, ":");
    if(pch == NULL) return -1;
    stime.tm_min = atoi(pch);

    pch = strtok(NULL, " ");
    if(pch == NULL) return -1;
    stime.tm_sec = atoi(pch);

    //rtcSetCounter(mktime(&stime));

    */

    return ecmdTime(NULL);
}

int tParse(char* ibuf, char** errorMsg)
{
    *errorMsg = 0;

    unsigned int i;
    char* pch;
    pch = strtok(ibuf, " "); 
    if(pch == NULL) return -1;

    for(i=0; i<(sizeof(ecmds)/sizeof(ecmds[0])); i++)
    {
        if(strcmpi(pch, ecmds[i].command) == 0)
            return (ecmds[i].func)(strtok(NULL, ""));
    }
    return 400;
}

