#include "novel.h"

void novel(int fd_ts, struct input_event tsEvent, char *novel_file, struct LcdDevice *lcd, font *f) //"xiaosuo.txt"
{
    FILE *txt_fp = fopen(novel_file, "r"); // 打开小说文件
    printf("%s\n", novel_file);
    if (txt_fp == NULL)
    {
        printf("打开小说文件失败\n");
    }

    FILE *np_fp = fopen("novel_position.txt", "w+"); // 打开小说位置信息文件
    if (np_fp == NULL)
    {
        printf("打开小说位置信息文件失败\n");
    }
    int txt_position = 0;
    getc(np_fp);
    if (feof(np_fp)) // 小说位置信息文件为空
    {
        fseek(np_fp, 0, SEEK_SET);
        while (1)
        {
            // 创建一个画板（点阵图）
            bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色
            // fontPrint(f, bm, 0, 0, gd_buf, getColor(0, 255, 255, 255), 100);// 将字体写到点阵图上
            // show_font_to_lcd(lcd->mp, 0, 0, bm);// 把字体框输出到LCD屏幕上
            // destroyBitmap(bm);// 关闭画板
            fprintf(np_fp, "%d\n", txt_position);
            printf("%d\n", txt_position);
            char novel_buf[90] = {0}; // 小说内容缓冲数组
            fseek(txt_fp, txt_position, SEEK_SET);
            for (int i = 0; i < 11; i++)
            {
                int k = 0;
                fscanf(txt_fp, "%[^\n]\n", novel_buf);
                printf("%s\n", novel_buf);
                fontPrint(f, bm, 0, k += i * 44, novel_buf, getColor(0, 0, 0, 0), 800); // 将字体写到点阵图上
                bzero(novel_buf, sizeof(novel_buf));
            }

            show_font_to_lcd(lcd->mp, 0, 0, bm); // 把字体框输出到LCD屏幕上
            destroyBitmap(bm);                   // 关闭画板
            if (feof(txt_fp))
            {
                printf("已到达小说末尾！\n");
                break;
            }
            txt_position = ftell(txt_fp);
            // sleep(1);
        }
    }

    fseek(txt_fp, 0, SEEK_SET);
    fseek(np_fp, 0, SEEK_SET);

    P_Node Head = Node_init(0);
    char buf[100] = {0};

    // printf("__%d__\n",__LINE__);
    while (fgets(buf, sizeof(buf), np_fp) != NULL) // 把小说位置偏移量信息加入到链表
    {
        Head->data = atoi(buf);
        P_Node new_node = Node_init(Head->data);
        Add2list(Head, new_node);
        printf("%d\n", Head->data);
        bzero(buf, sizeof(buf));
        // sleep(1);
    }

    txt_position = 0;

    while (1)
    {
        int direction = get_direction(fd_ts, tsEvent);
        if (direction == 3) // 左滑下一页
        {
            // 创建一个画板（点阵图）
            bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色

            printf("%d\n", txt_position);
            char novel_buf[90] = {0}; // 小说内容缓冲数组
            fseek(txt_fp, txt_position, SEEK_SET);
            for (int i = 0; i < 11; i++)
            {
                fscanf(txt_fp, "%[^\n]\n", novel_buf);
                printf("%s\n", novel_buf);
                fontPrint(f, bm, 0, i * 44, novel_buf, getColor(0, 0, 0, 0), 800); // 将字体写到点阵图上
                bzero(novel_buf, sizeof(novel_buf));
            }

            show_font_to_lcd(lcd->mp, 0, 0, bm); // 把字体框输出到LCD屏幕上
            destroyBitmap(bm);                   // 关闭画板
            if (feof(txt_fp))
            {
                printf("已到达小说末尾！\n");
                txt_position = 0;
            }
            else
            {
                txt_position = ftell(txt_fp);
            }
        }

        if (direction == 4) // 右滑上一页
        {
            // 创建一个画板（点阵图）
            bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色

            P_Node tmp = Node_init(0);
            list_for_each_entry(tmp, &Head->ptr, ptr)
            {
                if (tmp->data == txt_position)
                {
                    P_Node tmp_d = list_entry(tmp->ptr.prev->prev, Node, ptr);
                    txt_position = tmp_d->data;
                    // printf("%d\n", tmp->data);
                    break;
                }
            }

            printf("%d\n", txt_position);
            char novel_buf[90] = {0}; // 小说内容缓冲数组
            fseek(txt_fp, txt_position, SEEK_SET);
            for (int i = 0; i < 11; i++)
            {
                fscanf(txt_fp, "%[^\n]\n", novel_buf);
                printf("%s\n", novel_buf);
                fontPrint(f, bm, 0, i * 44, novel_buf, getColor(0, 0, 0, 0), 800); // 将字体写到点阵图上
                bzero(novel_buf, sizeof(novel_buf));
            }

            show_font_to_lcd(lcd->mp, 0, 0, bm); // 把字体框输出到LCD屏幕上
            destroyBitmap(bm);                   // 关闭画板
            if (feof(txt_fp))
            {
                printf("已到达小说末尾！\n");
                printf("%d\n", txt_position);
                tmp->data = 0;
                list_for_each_entry(tmp, &Head->ptr, ptr)
                {
                    if (tmp->data == txt_position)
                    {
                        P_Node tmp_d = list_entry(tmp->ptr.prev, Node, ptr);
                        txt_position = tmp_d->data;
                        // printf("%d\n", tmp->data);
                        break;
                    }
                }
            }
            else
            {
                txt_position = ftell(txt_fp);
            }
        }

        if (direction == 5)
        {
            break;
        }
    }

    P_Node tmp = Node_init(0);
    list_for_each_entry(tmp, &Head->ptr, ptr)
    {
        printf("%d\n", tmp->data);
    }
    Destory_list(Head);
    free(tmp);

    fclose(txt_fp);
    fclose(np_fp);
}