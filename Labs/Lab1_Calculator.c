// This is the lab 1: Unary sparse polynomial calculator  for DataStructure 2020 Fall by Bocheng Li.
// View my GitHub: https://github.com/BC-Li for more info. 

// include libraries
# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define OK 1;
# define ERROR 0;

typedef struct LinkedList{
    float data;
    int index;
    struct LinkedList *next; 
}SqList;

int ListInit(SqList *L){
    SqList *p = NULL, *end = NULL;
    int coefficient;
    int index_filler;
    end = L;
    L = (SqList *)malloc(sizeof(SqList));
    if(!L){
        return ERROR;
    }
    L->next = NULL;
    while((scanf("%d",&coefficient)!= EOF)&&(scanf("%d",&index_filler)!= EOF)){
        p = (SqList *)malloc(sizeof(SqList));
        p->data = coefficient;
        p->index = index_filler;
        end->next = p;
        end = p;
    }
    end->next = NULL;
    return OK;
}

int ListInsert(float e, int n, SqList *L ){
    SqList *p, *end;   // insert A+B to C 
    
    p = (SqList *)malloc(sizeof(SqList));
    if(!p){
        return ERROR;
    } 
    
    p->next = NULL;
    p->data = e;
    p->index = n;

    end = L;
    while(end!=NULL){
        end = end->next;
    }
    end->next = p;
    return OK;
    
}

int ListAddition(SqList *A, SqList *B, SqList *Addition){
    SqList *searcher_A = A,*searcher_B = B;
    int n = 0;
    float e = 0;
    Addition->next = NULL;
    while((searcher_A!=NULL)&&(searcher_B!=NULL)){

        if(searcher_A->index == n){
            e = e+searcher_A->data;
            searcher_A = searcher_A ->next;
        }

        if(searcher_B->index == n){
            e = e+searcher_B->data;
            searcher_B = searcher_B ->next;
        }

        ListInsert(e,n,Addition);
        e = 0;
        n++;
    }
    
    return OK;
}

int ListSubtraction(SqList *A,SqList *B, SqList *Substraction){    // A-B
    SqList *searcher_A = A,*searcher_B = B;
    int n = 0, e = 0;
    Substraction->next = NULL;
    while((searcher_A!=NULL)&&(searcher_B!=NULL)){

        if(searcher_A->index == n){
            e = e+searcher_A->data;
            searcher_A = searcher_A ->next;
        }

        if(searcher_B->index == n){
            e = e-searcher_B->data;
            searcher_B = searcher_B ->next;
        }

        ListInsert(e,n,Substraction);
        e = 0;
        n++;
    }
    
    return OK;
}

int PrintList(SqList *A){
    SqList *Traveler = A;
    if(Traveler->next == NULL){
        return ERROR;   // the list is empty or something else
    }
    while(Traveler != NULL){
        printf("%fx^%d",Traveler->data,Traveler->index);
    }
    return OK;
}

int DerivativeFunction(SqList *A){   // F'(x)
    SqList *Traveler = A;
    if(Traveler->next == NULL){
        return ERROR;   // the list is empty or something else
    }
    while(Traveler != NULL){
        printf("%fx^%d",Traveler->data*Traveler->index,Traveler->index-1);
    }
    return OK;
}

int CalculateValue(SqList *A, float x){     // Calculate the value of f(x) (given x)
    float Value = 0;
    SqList *Traveler = A;
    if(Traveler->next == NULL){
        return ERROR;
    }
    while(Traveler != NULL){

    }

}









