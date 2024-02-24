#include "ts.h"

int main(int argc, char const *argv[])
{
    int fd_ts = open("/dev/input/event0", O_RDONLY);
    if (fd_ts == -1)
    {
        perror("open event 0 error");
        return -1;
    }
    struct input_event tsEvent = {0};
    xy tmp_xy = {0};
    int direction = 0;
    while (1)
    {
        tmp_xy = get_xy(fd_ts, tsEvent, 2);
        printf("%d,%d\n", tmp_xy.x, tmp_xy.y);
        // direction = get_direction(fd_ts, tsEvent);
        // if (direction == 1)
        // {
        //     printf("上滑\n");
        // }
        // if (direction == 2)
        // {
        //     printf("下滑\n");
        // }
        // if (direction == 3)
        // {
        //     printf("左滑\n");
        // }
        // if (direction == 4)
        // {
        //     printf("右滑\n");
        // }

        // if (direction == 5)
        // {
        //     printf("返回\n");
        // }
    }
    close(fd_ts);
    return 0;
}
