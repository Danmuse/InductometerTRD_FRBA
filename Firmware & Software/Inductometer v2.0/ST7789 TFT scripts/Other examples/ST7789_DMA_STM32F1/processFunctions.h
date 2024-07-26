#ifndef _PROCESS_FUNCTIONS_H_
#define _PROCESS_FUNCTIONS_H_

#include "cardReader.h"
#include "st7789.h"

/// General settings panel

#define CARD_CONNECTION_ATTEMPTS 3

extern bool cardPlugged;

void printFromSD(const char *file);

#endif // _PROCESS_FUNCTIONS_H_
