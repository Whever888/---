#ifndef __NOVEL_H
#define __NOVEL_H

#include "ts.h"
#include "font.h"
#include "linkedList.h"
#include <string.h>

void novel(int fd_ts, struct input_event tsEvent, char *novel_file, struct LcdDevice *lcd, font *f); //

#endif