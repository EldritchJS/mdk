#pragma once

#include "DataTypes.h"

typedef struct
{
  void    (*cmd)(char*);
  char*   cmdName;
  char*   cmdInfo;
  char*   formatStr; 
  uint8_t numArgsExpectedMin;
  BOOL    framingDisable;
} CLI__S;

typedef union
{
  BOOL      b;
  char      c;
  int       d;
  uint8_t   u8;
   int8_t   i8;
  uint16_t  u16;
   int16_t  i16;
  uint32_t  u32;
   int32_t  i32;
  float     f;
  void*     vp;
  char      s[24+1];//s[200];
} CLI_ARGS__U;

extern char         fnNameCLI[];
extern CLI_ARGS__U  CLIArgs[];
extern uint8_t      CLINumArgs;

void CLIInit(const CLI__S* cmds, uint32_t numCmds);
void CLIProcess      (const CLI__S* cmds, uint32_t numCmds);
void CLIMenuPrint (const CLI__S* cmds, uint32_t numCmds);

BOOL CLIIsQuit(void);
BOOL CLIRx    (char* data);
