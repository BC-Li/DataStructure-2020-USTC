#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 20
#define FALSE 0
#define TRUE 1

typedef int Elemtype;
typedef struct Node{
    Elemtype data;
    struct Node* Next;

} Node;
typedef struct Node* LinkList;

int ListInsert(LinkList *L, int i, Elemtype e){
    int j;
    LinkList p,s;

    p = *L;
    j = 1;

    while(p&&j<i){
        p = p->Next;
        j++;
    }
    if(!p||j>i){
        return FALSE;
    }
    s=(LinkList)malloc(sizeof(Node));
    s->data = e;

    s->Next = p->Next;
    p->Next = s;
    return TRUE;
}

int ListDelete(LinkList *L,int i, Elemtype *e){
    int j;
    LinkList p,q;
    p=*L;
    j=1;

    while(p->Next && j<i){
        p=p->Next;
        j=j+1;
    }
    if(!(p->Next)||j>i){
        return FALSE;
    }

    q = p->Next;
    p->Next = q->Next;

    *e = q->data;
    free(q);
    
    return TRUE;
}