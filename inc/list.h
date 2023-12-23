#ifndef __list_h__
#define __list_h__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Kernel_List.h"
#include<stdbool.h>

typedef struct DataType
{
    int num;
    char name[500];
}DataType, *P_DataType;

typedef struct Node
{
    DataType Data;
    struct list_head ptr;
}Node, *P_Node;

P_Node NodeInit(P_DataType new);


DataType GetNewData();

void Add2List(P_Node Head, P_Node NewNode);

P_Node Delete(P_Node Head);


// int main(int argc, char const *argv[])
// {
//     P_Node Head = NodeInit(NULL);
//     DataType list[5] = {
//                         [0].num = 12, [0].name = "🤢",
//                         [1].num = 89, [1].name = "😚",
//                         [2].num = 76, [2].name = "👺",
//                         [3].num = 56, [3].name = "🍅",
//                         [4].num = 23, [4].name = "🤩",
//                         };
//     for (int i = 0; i < 5; i++)
//     {
//         // DataType NewData = GetNewData();
//         // P_Node NewNode = NodeInit(&NewData);
//         P_Node NewNode = NodeInit(list+i);
//         Add2List(Head, NewNode);
//     }
//     struct list_head *pos = NULL;
//     list_for_each(pos, &Head->ptr)
//     {
//         P_Node tmp = list_entry(pos, Node, ptr);
//         printf("[%d] %s\n",tmp->Data.num, tmp->Data.name);
//     }
//     Delete(Head);
//     list_for_each(pos, &Head->ptr)
//     {
//         P_Node tmp = list_entry(pos, Node, ptr);
//         printf("[%d] %s\n",tmp->Data.num, tmp->Data.name);
//     }
//     return 0;
// }

#endif

