#include "Kernel_List.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct Node
{
    int data;
    struct list_head ptr;
} Node, *P_Node;

P_Node Node_init(int data);
void Add2list(P_Node Head, P_Node New_node);
void Destory_list(P_Node Head);