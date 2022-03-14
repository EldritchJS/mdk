#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "DataTypes.h"

#define TRACE ConsoleTxStr
#define TRACERx ConsoleRxChar
#define TRACEStr TRACE

void ConsoleInit          (void);
void ConsoleTxStr         (char* str); 
BOOL ConsoleRxChar        (char* data);
void ConsoleService(void);

#ifdef __cplusplus
}
#endif

