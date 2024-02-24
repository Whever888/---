#include "ts.h"

xy get_xy(int fd_ts, struct input_event tsEvent, int mode)
{
    xy tmp_xy = {0};

    int x_tmp = 0;
    int y_tmp = 0;

    bool x_flag = false;
    bool y_flag = false;

    if (mode == 1) // 获取点击事件的坐标
    {
        while (1)
        {
            read(fd_ts, &tsEvent, sizeof(tsEvent));
            if (tsEvent.type == EV_ABS)
            {
                if (tsEvent.code == ABS_X)
                {
                    x_tmp = tsEvent.value * 800 / 1024;
                }
                else if (tsEvent.code == ABS_Y)
                {
                    y_tmp = tsEvent.value * 480 / 600;
                }
            }

            if (tsEvent.type == EV_KEY &&
                tsEvent.code == BTN_TOUCH &&
                tsEvent.value == 1)
            {
                tmp_xy.x = x_tmp;
                tmp_xy.y = y_tmp;
                return tmp_xy;
            }
        }
    }
    else if (mode == 2) // 直接返回一对坐标
    {
        while (1)
        {
            read(fd_ts, &tsEvent, sizeof(tsEvent));
            if (tsEvent.type == EV_ABS)
            {
                if (tsEvent.code == ABS_X)
                {
                    x_tmp = tsEvent.value * 800 / 1024;
                    x_flag = true;
                }
                else if (tsEvent.code == ABS_Y)
                {
                    y_tmp = tsEvent.value * 480 / 600;
                    y_flag = true;
                }
            }
            if (x_flag && y_flag)
            {
                tmp_xy.x = x_tmp;
                tmp_xy.y = y_tmp;
                return tmp_xy;
            }
        }
    }
}

int get_direction(int fd_ts, struct input_event tsEvent)
{
    int x_start = 0;
    int y_start = 0;

    int x_end = 0;
    int y_end = 0;

    int x_tmp = 0;
    int y_tmp = 0;

    while (1)
    {
        read(fd_ts, &tsEvent, sizeof(tsEvent));
        if (tsEvent.type == EV_ABS)
        {
            if (tsEvent.code == ABS_X)
            {
                x_tmp = tsEvent.value * 800 / 1024;
            }
            else if (tsEvent.code == ABS_Y)
            {
                y_tmp = tsEvent.value * 480 / 600;
            }
        }

        if (tsEvent.type == EV_KEY &&
            tsEvent.code == BTN_TOUCH &&
            tsEvent.value == 1)
        {
            x_start = x_tmp;
            y_start = y_tmp;

            if (x_tmp > 750 && y_tmp < 50)
            {
                return 5; // 返回5说明按到返回键了
            }
        }

        if (tsEvent.type == EV_KEY &&
            tsEvent.code == BTN_TOUCH &&
            tsEvent.value == 0)
        {
            x_end = x_tmp;
            y_end = y_tmp;
        }

        if (x_end != 0 && x_start != 0) // 如果松手x坐标不为0，代表一次滑动完成，进行方向判断
        {
            if (y_end - y_start < -50)
            {
                return 1; // 上滑
            }
            else if (y_end - y_start > 50)
            {
                return 2; // 下滑
            }
            else if (x_end - x_start < -100)
            {
                return 3; // 左滑
            }
            else if (x_end - x_start > 100)
            {
                return 4; // 右滑
            }
        }
    }
}
