#include <sdk.h>
#include "CLI.h"
#include "console.h"
#include "version.h"

#define CONSOLE_UART 0

static CLI__S menuCmds[]=
{
  // function pointer               // name                     // description                                      // argument parse string      // num args   // disable begin/DONE trace
  {&VersionCLI,                     "Version",                  VERSION_STR,                                        "%s",                         0, 0},
};

void ConsoleTxStr (char* str)   
{         
	for(unsigned int i=0; i<strlen(str); i++)
	{
		uart_write(CONSOLE_UART, str[i]);           
	}
}

BOOL ConsoleRxChar(char* data)  
{ 
	return  uart_read(CONSOLE_UART,(uint8_t *) data); 
}

uint8_t TRACEStrOption(char* str)
{  
  ConsoleTxStr(str);
  
  return 1; 
}

#if 0
BOOL TRACERx(char* data)
{
  return ConsoleRxChar(data);
}
#endif

void ConsoleInit(void)
{
}

void ConsoleService(void)
{
  static BOOL initialized = FALSE;
  
  if(initialized == FALSE)
  {
    CLIInit(menuCmds, sizeof(menuCmds)/sizeof(CLI__S));
    initialized = TRUE;
  }
  CLIProcess(menuCmds, sizeof(menuCmds)/sizeof(CLI__S));
}

