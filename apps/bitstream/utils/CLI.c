#include "DataTypes.h"
#include "CLI.h"
#include "stdio.h"
#include "string.h"
#include "console.h"

#define CMD_LINE_LENGTH     110
#define CMD_HISTORY_LENGTH  4

char fnNameCLI[100];

CLI_ARGS__U CLIArgs[25] = {0};
uint8_t     CLINumArgs = 0;
void        (*CLICmd)(char*);

static  uint32_t  lineI = 0;
static  char      cmdLineHistory[CMD_HISTORY_LENGTH][CMD_LINE_LENGTH];
static  uint8_t   cmdLineHistoryI = 0;

// set bufSize==0 to initialize
static void SetLine(uint32_t bufSize)
{
  lineI = bufSize;
}

BOOL GetLine(char c, char* buf, uint32_t bufSize)
{
  BOOL doneFlag = FALSE;

  if(bufSize==0)      
  { 
    lineI=0;   
  }
  else if (lineI>=bufSize)  
  { 
    lineI=0; 
    TRACE("ERROR: line too long\r\n"); 
  }

  else if (c=='\n')         { buf[lineI]=0; lineI=0; doneFlag=TRUE;       }
  else if (c=='\r')         { buf[lineI]=0; lineI=0; doneFlag=TRUE;       }
  else if (c=='\b')         { if(lineI>0) { --lineI; }                    }
  else                      { buf[lineI++]=c;                             }

  return doneFlag;
}

void CLIMenuPrint(const CLI__S* cmds, uint32_t numCmds)
{
  uint32_t  i=0;

  TRACE("\r\n");
  TRACE("*************************************************************\r\n");
  TRACE("************************ CLI Menu ***************************\r\n");
  TRACE("*************************************************************\r\n");
  for(i=0; i<numCmds; i++)
  {
    TRACE(cmds[i].cmdName);
    TRACEStr(cmds[i].cmdInfo);
    TRACE("\r\n");
    //TimerMSDelay(1+ ((strlen(cmds[i].cmdInfo)+strlen(cmds[i].cmdName))/10)); // keep tx buffer from overrunning at 115200 baud
  }
  TRACE("    '?' <ENTER> for help\r\n");
  TRACE("*************************************************************\r\n");
}

static BOOL CmdRun(const CLI__S* cmds, uint32_t numCmds, char* cmdLine)
{
  static  char      cmd[CMD_LINE_LENGTH];
  uint32_t  i=0;
  BOOL      foundFlag = TRUE;
  
  cmd[0]=0;
// addback  sscanf(cmdLine,"%s",cmd);

  for(i=0; i<numCmds; i++)
  {
    if(strcmp(cmd,cmds[i].cmdName)==0)
    {
      TRACE("\r\n");
      // run CLI test framework if formatStr is defined
      cmds[i].cmd(cmdLine);
      break;
    }
    else if(cmdLine[0]=='?') 
    { 
      CLIMenuPrint(cmds,numCmds);
      break;
    }
  }
  
  if(i==numCmds) 
  { 
    foundFlag = FALSE; 
    TRACE("ERROR: cmd not found\r\n");
  }

  return foundFlag;
}

void CLIInit(const CLI__S* cmds, uint32_t numCmds)
{
  CLIMenuPrint(cmds,numCmds);  
}

void CLIProcess(const CLI__S* cmds, uint32_t numCmds)
{
  static  BOOL  initializedFlag = FALSE;
  static  char  line[CMD_LINE_LENGTH];  
          char  c = 0;

  if(!initializedFlag)
  {
    initializedFlag = TRUE;
    memset(line,0,sizeof(line));
    memset(cmdLineHistory,0,sizeof(cmdLineHistory));
  }
          
  while(TRACERx(&c))
  {
    if(c=='+')
    {
      cmdLineHistoryI = (uint8_t)((cmdLineHistoryI == 0) ? CMD_HISTORY_LENGTH-1 : cmdLineHistoryI-1);
      strcpy(line,cmdLineHistory[cmdLineHistoryI]);
      SetLine(strlen(line));
      // clear line
      TRACE("\r");
      TRACE("                                                                ");
      TRACE("\r");
      // display previous command
      TRACEStr(line);
    }
    else if(GetLine(c,line,sizeof(line)))
    {
      if(strlen(line)>0)
      {
        strcpy(cmdLineHistory[cmdLineHistoryI],line);
        cmdLineHistoryI = (uint8_t)((cmdLineHistoryI < (CMD_HISTORY_LENGTH-1)) ? cmdLineHistoryI+1 : 0);
        CmdRun(cmds,numCmds,line);
        memset(line,0,sizeof(line));
      }
    }
  }
}

BOOL CLIIsQuit(void)
{
  char data;

  if(TRACERx(&data))
  {
    if(data=='q') { return TRUE; }
  }

  return FALSE;
}

BOOL CLIRx(char* data)
{
  return TRACERx(data);
}

