#ifndef __DIR_SEARCH_H
#define __DIR_SEARCH_H

#include "list.h"

#define IS_DIR 4
#define IS_REG 8

int dt_dir(char const *dir, const char* type, P_Node Head);


#endif