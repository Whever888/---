#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <sys/select.h>

int main(int argc, char const *argv[])
{
    int fd_ts = open("/dev/input/event0", O_RDONLY);
    if (fd_ts == -1)
	{
		perror("open event 0 error");
		return -1;
	}

    struct input_event tsEvent = {0};

	int x_start = 0;
	int y_start = 0;

    int x_tmp = 0;
	int y_tmp = 0;

	int x_end = 0;
	int y_end = 0;
    while (1)
    {
        int ret_val = read(fd_ts, &tsEvent, sizeof(tsEvent));
        if (tsEvent.type == EV_ABS)
        {
            if (tsEvent.code == ABS_X)
            {
                x_tmp = tsEvent.value * 800 / 1024;
            }
            else if(tsEvent.code == ABS_Y)
            {
                y_tmp = tsEvent.value *480 / 600;
            }
        }

        if (tsEvent.type == EV_KEY &&
            tsEvent.code == BTN_TOUCH)
        {

            if (tsEvent.value == 1 )
            {
                x_start = x_tmp;
                y_start = y_tmp;
                printf("start[%d][%d]\n",x_start,y_start);
            }

            if (tsEvent.value == 0 )
            {
                x_end = x_tmp;
                y_end = y_tmp;
                printf("end[%d][%d]\n",x_end,y_end);
                if (x_start - x_end > 200 && abs(y_start - y_end) < 60)//*************左滑****************
                {
                    printf("左滑一次\n");
                }

                if (x_start - x_end < -200 && abs(y_start - y_end) < 60)//*************右滑****************
                {
                    printf("右滑一次\n");
                }
            }

        } 
    }
    
    
    
    return 0;
}

