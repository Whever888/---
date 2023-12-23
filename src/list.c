#include"list.h"

P_Node NodeInit(P_DataType new)
{
    P_Node NewNode = calloc(1,sizeof(Node));
    if (NewNode == NULL)
    {
        printf("申请堆内存失败！\n");
        return NULL;
    }
    if (new != NULL)
    {
        memcpy(NewNode, new, sizeof(DataType));
    }
    INIT_LIST_HEAD(&NewNode->ptr);
    return NewNode;
}

DataType GetNewData()
{
    DataType NewNode = {0};
    if (scanf("%d %s",&NewNode.num,NewNode.name) != 2)
    {
        printf("输入错误！\n");
        while (getchar() != '\n');
    }
    return NewNode;
}

void Add2List(P_Node Head, P_Node NewNode)
{
    list_add(&NewNode->ptr, &Head->ptr);
}

P_Node Delete(P_Node Head)
{
    int DelNum = 0;
    P_Node pos = NULL, n = NULL;
    printf("请输入要删除的节点编号：\n");
    scanf("%d",&DelNum);
    list_for_each_entry_safe(pos, n, &Head->ptr, ptr)
    {
        if (pos->Data.num == DelNum)
        {
            list_del(&pos->ptr);
        }
        // printf("[%d]-----------%s\n",pos->Data.num, pos->Data.name);
    }
    
}