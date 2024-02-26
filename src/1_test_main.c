#include "ts.h"
#include"novel.h"

int main(int argc, char const *argv[])
{
    //***************小说************
    struct LcdDevice *lcd = init_lcd("/dev/fb0"); //(font的lcd初始化)               
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");// 打开字体
    fontSetSize(f, 44);// 字体大小的设置

    //***************触摸屏***********
    int fd_ts = open("/dev/input/event0", O_RDONLY);
    if (fd_ts == -1)
    {
        perror("open event 0 error");
        return -1;
    }

    struct input_event tsEvent = {0};
    xy tmp_xy = {0};
    int direction = 0;

    //*****************主函数*******************
    novel(fd_ts, tsEvent, "xiaosuo.txt", lcd, f);//
    close(fd_ts);
    return 0;
}
