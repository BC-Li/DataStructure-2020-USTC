#include<stdio.h>
#include<stdlib.h>

# define ERROR -1;
# define OK 1;

typedef int ElemType;
typedef struct Linkedlist{
    ElemType data;
    struct Linkedlist *next;
} Sqlist;

int ListInit(Sqlist *L, int n){
    Sqlist p;
    int i ;
    p = (Sqlist *)malloc(sizeof(Sqlist));
    if(p == NULL){
        return ERROR;
    }
    for(int i = 1;i<10;i++){
        
    }
        return OK;
    
}

int ListInsert(Sqlist *p, int x){
    Sqlist *q;
    q = (Sqlist *)malloc(sizeof(Sqlist));
    p->next = q;

}
