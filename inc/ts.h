#ifndef __TS_H
#define __TS_H

#include <fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#include <linux/input.h> 

#include<unistd.h>
#include<stdio.h>

typedef struct xy
{
    int x;
    int y;
} xy, *P_xy;

xy get_xy(int fd_ts, struct input_event tsEvent);
int get_direction(int fd_ts, struct input_event tsEvent);

#endif