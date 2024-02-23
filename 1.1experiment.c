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
    FILE *txt = fopen("xiaosuo.txt", "r");//打开小说文件
    if (txt == NULL)
    {
        printf("打开“txt”失败\n");
    }

    FILE *novel_position = fopen("novel_position.txt", "r+");//打开小说位置信息文件
    if (novel_position == NULL)
    {
        printf("打开“novel_position.txt”失败\n");
    }
    int page_in_total = 0;//总页数
    int txt_position = 0;//小说当前位置
    char novel_position_buf[10] = {0}; //用于计算页数的缓冲区
    
     while (1)//遍历位置信息文件计算总页数（如果第一次打开此文件则为0，否则为非0）
    {
        if (fgets(novel_position_buf, 10, novel_position) != NULL)
        {
            page_in_total++;
        }
        else
        {
            break;
        }
    }
    int page_index[page_in_total];//小说位置偏移量数组
   
    bzero(page_index, page_in_total);//清空小说位置偏移量数组  
    printf("total[%d]\n", page_in_total);
    fscanf(novel_position, "%d", &txt_position);//读取第一行小说当前位置信息

    if (txt_position == 0 && page_in_total == 0)//小说当前位置为0且位置信息文件总页数为0表示第一次打开这本小说
    {
        fprintf(novel_position, "%d\n", txt_position);//把小说当前位置信息写在位置文件第一行

        while (1)
        {
            fprintf(novel_position, "%d\n", txt_position);//把小说第一页位置信息写在位置文件第二行，并把后续偏移量继续写入位置文件
            char novel_buf[80] = {0};//小说内容缓冲数组
            for (int i = 0; i < 11; i++)//***********************************以下是打印区
            {
                int k = 0;
                fscanf(txt,  "%[^\n]\n", novel_buf);                 
                printf("[%s]\n", novel_buf);
            }//********************************************以上是打印区

            txt_position = ftell(txt);//下一页的起始位置

             if(feof(txt))
            {
                printf("文件内容已读完.\n");
                txt_position = 0;
                break;
            }
            
        }
    }
    else
    {
        fseek(novel_position, 0, SEEK_SET);
        for (int i = 0; i < page_in_total; i++)//把位置信息文件偏移量加入数组
        {
            fscanf(novel_position, "%d", &page_index[i]);
            printf("page_index[%d]=[%d]\n", i, page_index[i]);
        }

    }
    printf("--------------------------\n");
    // fseek(txt, page_index[1], SEEK_SET);
    // char novel_buf[80] = {0};
    // for (int i = 0; i < 11; i++)//***********************************以下是打印区
    // {
    //     int k = 0;
    //     fscanf(txt,  "%[^\n]\n", novel_buf);                 
    //     printf("[%s]\n", novel_buf);
    // }//********************************************以上是打印区


    while (1)
    {
        // int i = page_in_total-1;
        int i = 0;
        for (; i < page_in_total; i++)//把位置信息文件偏移量加入数组
        {
            printf("page_index[%d]=[%d]\n", i, page_index[i]);
            char novel_buf[80] = {0};//小说内容缓冲数组
            fseek(txt, page_index[i], SEEK_SET);
            for (int i = 0; i < 11; i++)//***********************************以下是打印区
            {
                int k = 0;
                fscanf(txt,  "%[^\n]\n", novel_buf);                 
                printf("[%s]\n", novel_buf);
            }//********************************************以上是打印区
          
        }
        if (i == page_in_total)
        {
            break;
        }
    }

printf("******************************************************************************\n");
int buf[14] = {0,
696,
1470,
2246,
3028,
3847,
4655,
5394,
6161,
6923,
7682,
8450,
9229,
9992};
    for (int k = 0; k < 14; k++)//**********************************************
        {
            txt_position = buf[k];
        
        
            for (int i = 1; i < page_in_total; i++)//查找当前位置在偏移量数组中的位置，偏移到上一页
            {
                if (page_index[i] == txt_position && i > 2 ) // p2 	  		  p3   			p14
                {	
                    fseek(txt, page_index[i-2], SEEK_SET);
                    printf("end1[%d]-[%ld]\n", i, ftell(txt));
                }
                else if (page_index[i] == txt_position && i == 1 )
                {	
                    fseek(txt, page_index[13], SEEK_SET);
                    printf("end2[%d]-[%ld]\n", i, ftell(txt));
                }
                else if (page_index[i] == txt_position && i == 2)
                {	
                    fseek(txt, page_index[13], SEEK_SET);
                    printf("end3[%d]-[%ld]\n", i, ftell(txt));
                }
                
            }
        }//*************************************************  
        
    // fseek(novel_position, 0, SEEK_SET);
	// fprintf(novel_position, "%s", "                         ");
    // fseek(novel_position, 0, SEEK_SET);
	// fprintf(novel_position, "%d", txt_position);

    fclose(txt);
	fclose(novel_position);	
    return 0;
}

