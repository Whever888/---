#include "touch.h"

int touch_init(Touch *touch)
{
    //打开/dev/input/event0文件，以只读的方式，非阻塞模式
    touch->touch_fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    //如果打开失败，打印错误信息，关闭文件描述符，返回-1
    if (touch->touch_fd == -1)
    {
        perror("open");
        close(touch->touch_fd);
        return -1;
    }
    //分配touch->start和touch->end结构体，并初始化
    touch->start = (struct coor *)calloc(1, sizeof(struct coor));
    touch->end = (struct coor *)calloc(1, sizeof(struct coor));
    //如果分配失败，打印错误信息，关闭文件描述符，释放touch->start和touch->end结构体，返回-1
    if (touch->start == NULL || touch->end == NULL)
    {
        perror("calloc");
        close(touch->touch_fd);
        free(touch->start);
        free(touch->end);
        return -1;
    }
    //初始化touch->start和touch->end结构体的x和y坐标
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    //初始化touch->status和touch->pos
    touch->status = -1;
    touch->pos = 0;
    //分配touch->buf和touch->time结构体，并初始化
    touch->buf = (struct input_event *)calloc(1, sizeof(struct input_event));
    touch->time = (struct timeval *)calloc(1, sizeof(struct timeval));
    //返回0
    return 0;
}

Touch *touch_open()
{
    // 分配内存空间
    Touch *touch = (Touch *)calloc(1, sizeof(Touch));
    // 如果分配失败，则打印错误信息，并返回NULL
    if (touch == NULL)
    {
        perror("calloc");
        return NULL;
    }
    // 初始化touch信息
    else if (touch_init(touch) == -1)
    {
        perror("touch_info");
        free(touch);
        return NULL;
    }
    // 返回touch指针
    return touch;
}




// 关闭触摸屏
int touch_close(Touch *touch)
{
    close(touch->touch_fd);
    free(touch->start);
    free(touch->end);
    free(touch->buf);
    free(touch);
    return 0;
}
//触摸屏信息输出
void touch_info(Touch *touch);


int touch_start(Touch *touch, const unsigned int mode)
{
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    touch->status = 0;
    touch->pos = 0;
    while (1)
    {
        if (touch_check_timeout(touch) == 0)
        {
            continue;
        }
        if (read(touch->touch_fd, touch->buf, sizeof(*touch->buf)) == -1)
        {
            perror("read");
            return -1;
        }
        // 记录手指按下时的坐标
        if (touch->buf->type == EV_ABS && touch->buf->code == ABS_X)//判断是否发生X轴绝对位移事件
        {
            if (!touch->start->x)
                touch->start->x = touch->buf->value * 800 / 1024;
            touch->end->x = touch->buf->value * 800 / 1024;
        }
        if (touch->buf->type == EV_ABS && touch->buf->code == ABS_Y)//判断是否发生y轴绝对位移事件
        {
            if (!touch->start->y)
                touch->start->y = touch->buf->value * 480 / 600;
            touch->end->y = touch->buf->value * 480 / 600;
        }
        if ((touch->buf->type == EV_KEY && touch->buf->code == BTN_TOUCH && touch->buf->value == 0) &&
            (touch->start->x * touch->start->y * touch->end->x * touch->end->y != 0) && mode == 0)//松手检测
        {
            int tran = touch->end->x - touch->start->x, dire = touch->end->y - touch->start->y;
            // 判断这俩数的绝对值谁大
            if ((abs(tran) > abs(dire)) && abs(tran) > 20)
            {
                touch->status = 1;
                touch->pos = (tran > 0) ? (1) : (-1);
                return 1;
            }
            else if ((abs(dire) > abs(tran)) && abs(dire) > 20)
            {
                touch->status = 1;
                touch->pos = (dire < 0) ? (2) : (-2);
                return 1;
            }
            // 活动范围过小
            touch->status = 1;
            return 0;
        }
        else if ((touch->start->x * touch->start->y * touch->end->x * touch->end->y != 0) && mode == 1)
        {
            // 没有松手检测拖动，持续输出
            touch->status = 1;
            return 0;
        }
        else if (touch->start->x != 0 && touch->start->y != 0 && mode == 2)
        {
            // 没有抬手检测的点击，不支持拖动
            touch->status = 1;
            return -1;
        }
    }
    return -1;
}










int touch_clear(Touch *touch)
{
    // 检查输入的指针是否为空
    if (touch == NULL)
        return -1;
    // 如果超时，则读取指针，并重置指针的值
    while (touch_check_timeout(touch) != 0 && read(touch->touch_fd, touch->buf, sizeof(*touch->buf)) != -1);
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    touch->status = 0;
    touch->pos = 0;
    return 0;
}



// 检测查询是否超时
int touch_check_timeout(Touch *touch)
{
    //检查touch是否为空
    if (touch == NULL)
        return -1;
    //定义一个文件描述符集合
    fd_set fds;
    //清空文件描述符集合
    FD_ZERO(&fds);
    //将touch的文件描述符添加到文件描述符集合中
    FD_SET(touch->touch_fd, &fds);
    //设置超时时间
    touch->time->tv_sec = 0;
    touch->time->tv_usec = 1000;
    //调用select函数，检查文件描述符集合
    int ret = select(touch->touch_fd + 1, &fds, NULL, NULL, touch->time);
    //检查select函数的返回值
   // 检查select函数的返回值
    if (ret == -1)
    {
        // 如果返回值为-1，则表示select函数调用失败
        perror("select error");
        return -1;
    }
    else if (ret == 0)
    {
        // 屏幕缓存区空
        return 0;
    }
    // 屏幕缓存区没空
    return 1;
}



