#include"linkedList.h"
#include<stdio.h>

int main(int argc, char const *argv[])
{
    P_Node head = Node_init(0);
    P_Node n1 = Node_init(10);
    P_Node n2 = Node_init(5);
    P_Node n3 = Node_init(40);
    Add2list(head, n1);
    Add2list(head, n2);
    Add2list(head, n3);

    P_Node tmp = Node_init(0);
    list_for_each_entry(tmp, &head->ptr, ptr)
    {
       printf("%d\n", tmp->data); 
    }
    Destory_list(head);
    // printf("tmp %lu\n", sizeof(tmp));
    free(tmp);
    
    return 0;
}
