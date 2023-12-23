#include "dir_search.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int dt_dir(char const *dir, const char* type, P_Node Head)
{
    DIR *o_dir = opendir(dir);
    char tmp[500] = {0};

    if (o_dir == NULL)
    {
        // fprintf(stderr,"打开%s失败！%s\n",dir,strerror(errno));
        return -1;
    }
    while (1)
    {
        struct dirent *read_dir = readdir(o_dir);
        if (read_dir == NULL)
        {
            // fprintf(stdout, "%s\n", strerror(errno));
            break;
        }
        if (read_dir->d_type == IS_DIR && read_dir->d_name[0] != '.')
        {

            snprintf(tmp, 500, "%s/%s", dir, read_dir->d_name);
            dt_dir(tmp, type, Head);
        }
        if (read_dir->d_type == IS_REG)
        {
            char *ptr = strrchr(read_dir->d_name,'.');
            if (ptr && !strcmp(ptr , type))
            {
                DataType data = {.num = 0};
                strcpy(data.name, read_dir->d_name);
                P_Node NewNode = NodeInit(&data);
                Add2List(Head, NewNode);
                printf("正在加入链表[%s][%s]\n", dir, read_dir->d_name);
            }
            
        }
    }
    return 0;
}

// int main(int argc, char const *argv[])
// {
//     FILE *file1 = fopen("dir.txt", "r+");
//     P_Node Head = NodeInit(NULL);
//     if (argc != 2)
//     {
//         printf("输入错误！\n");
//         return -1;
//     }
//     dt_dir(argv[1], ".c", Head);
//     struct list_head *pos = NULL;
//     list_for_each(pos, &Head->ptr)
//     {
//         P_Node tmp = list_entry(pos, Node, ptr);
//         printf("[%d] %s\n",tmp->Data.num, tmp->Data.name);
//         fprintf(file1, "[%d]-[%s]\n", tmp->Data.num, tmp->Data.name);
//     }
//     printf("***************************\n");
//     DataType tmp = {.num = 3, .name = "fff"};
//     fseek(file1, 0, SEEK_SET);
//     while (fscanf(file1, "[%d]-[%s\n", &tmp.num, tmp.name) != EOF)
//     {
//         printf("[%d]-[%s\n", tmp.num, tmp.name);
//         sleep(1);
//     }
//     fclose(file1);
//     return 0;
// }
