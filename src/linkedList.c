#include "linkedList.h"

P_Node Node_init(int data)
{
    P_Node New_node = calloc(1, sizeof(Node));
    if (New_node == NULL)
    {
        printf("申请堆内存失败！\n");
        return NULL;
    }
    New_node->data = data;
    INIT_LIST_HEAD(&New_node->ptr);
    return New_node;
}

void Add2list(P_Node Head, P_Node New_node)
{
    list_add_tail (&New_node->ptr, &Head->ptr);
}

void Destory_list(P_Node Head)
{
    while (!list_empty(&Head->ptr))
    {
        P_Node tmp_node = list_first_entry(&Head->ptr, Node, ptr);
        tmp_node->data = 0;
        free(tmp_node);
        list_del(&tmp_node->ptr);
    }
    // free(Head);
    // list_del(&Head->ptr);
}