#ifndef __TOUCH_H
#define __TOUCH_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <sys/select.h>

struct coor
{
    int x;
    int y;
};

struct touch
{
    int touch_fd;
    struct coor *start;
    struct coor *end;
    int status; //0是未点击，1是点击
    int pos; //位置1是右，-1是左，2是上，-2是下
    struct input_event *buf;
    struct timeval *time;
};
typedef struct touch Touch;

// 打开触摸屏
Touch *touch_open();
// 关闭触摸屏
int touch_close(Touch *touch);


//触摸屏信息输出
void touch_info(Touch *touch);





//触摸，mode=0支持方向按，同时兼顾点击，mode=1没有松手检测拖动，持续输出，mode=2，没有抬手检测的点击，不支持拖动
int touch_start(Touch *touch, const unsigned int mode);
//清除触摸点
int touch_clear(Touch *touch);

//检查触摸点超时，1缓存区没空，0缓存区空，-1读取错误
int touch_check_timeout(Touch *touch);
#endif

