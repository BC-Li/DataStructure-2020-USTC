// This is the lab 1: Unary sparse polynomial calculator for DataStructure 2020 Fall by Bocheng Li.
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

int PrintList(SqList *A);

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
    printf("Please input the coefficient and index. following order like '1 2'. press ctrl+z to stop.");
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

int OutputListInit(SqList *L){  // Init a linkedlist which is used for storing the outputs.
    L = (SqList *)malloc(sizeof(SqList));
    if(!L){
        return ERROR;
    }
    L->data = 0;
    L->index = 0;
    L->next = NULL;
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
    while(end->next!=NULL){
        end = end->next;
    }
    end->next = p;
    return OK;
    printf("Insert ready.\n");
}

// Calculate functions for LinkedList 

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

int ListMultiply(SqList *A, SqList *B, SqList *MultiplyTemp, SqList *Multiply_B, SqList *Output){
    OutputListInit(MultiplyTemp);
    OutputListInit(Output);
    SqList *searcher_A = A, *searcher_B = B;
    while(searcher_A!=NULL){
        while(searcher_B!=NULL){
            int MultiplyedData = searcher_A->data * searcher_B->data;
            int MultipliedIndex = searcher_A->index + searcher_B->index;
            ListInsert(MultiplyedData,MultipliedIndex,MultiplyTemp);
        }
        ListAddition(MultiplyTemp,Output,Output);
    }
    PrintList(Output);
    return OK;
} 

//  Standard Output and Other Features

int PrintList(SqList *A){       // reverse print using recursive func
    PrintList(A->next);
    if(A->next == NULL){
        return ERROR;   // the list is empty or something else
    }

    // Head of the polynomial, add some special judgements 
    if(A->data > 0){     
        printf("%fx^%d",A->data,A->index);
    }
    if(A->data > 0){     
        printf("-%fx^%d",A->data,A->index);
    }
    
    // Print the remaining factors
    while(A != NULL){
        if(A->data > 0){
            printf("+%fx^%d",A->data,A->index);
        } 
        if(A->data < 0){
            printf("%fx^%d",A->data,A->index);
        }
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
        return ERROR;   // the list is empty or something else
    }
    while(Traveler != NULL){
        Value = Value + Traveler->data*(pow(x,Traveler->data));
    }
    return OK;
}

int ListFree(SqList *L){
    SqList *p;
    while(L!=NULL){
        p = L->next;
        free(L);
        L = p;
    }
    printf("free.\n");
}
void welcome(){
	printf ("\n");
	printf ("\n");
	printf ("\n");
    printf("***************************Unary sparse polynomial calculator By Bocheng Li*****************************");
    printf("***************************View https://github.com/BC-Li for more info.*********************************");
    printf("\n");
    printf("\n");
    printf("********************************    1.INIT      ***************************************\n");
    printf("********************************    2.Subtraction      ***************************************\n");
    printf("********************************    3.Multiply      ***************************************\n");
    printf("********************************    4.Display  ***************************************\n");
    printf("********************************    0.EXIT    ***************************************\n\n\n");
 
}
int main() 
{ 
    SqList A,B,Addition;
    ListAddition(&A,&B,&Addition);
    PrintList(&Addition);
    return 0;
    // char  choice[100];
    // int choice_1;
    // char  yes_no;
    
    
    // do
    // {
    //     welcome();
    // printf("*******************************    Input your command  0-5:  **************************************\n");
    //     scanf("%s",choice);
    //     choice_1=choice[0]-48;
    //     printf("\n");
    //     switch(choice_1)
    //    {
    //         case 1:{
    //             SqList A, B;
    //             ListInit(&A);
    //             ListInit(&B);
    //             break;
    //         }
    //         case 2:{
    //             SqList A, B;
    //             ListInit(&A);
    //             ListInit(&B);
    //             SqList AdditionResult;
    //             ListAddition(&A,&B,&AdditionResult);
    //             PrintList(&AdditionResult);
    //             break;
    //         }
    //         case 3:{
    //             SqList A, B;
    //             ListInit(&A);
    //             ListInit(&B);
    //             SqList SubtractionResult;
    //             ListSubtraction(&A,&B,&SubtractionResult);
    //             PrintList(&SubtractionResult);
    //             break;
    //         }
    //         case 4:{
    //             SqList A, B;
    //             ListInit(&A);
    //             ListInit(&B);
    //             SqList MultiplyResult;
    //             // ListMultiply(&A,&B,&MultiplyTemp, SqList *Multiply_B, SqList *Output);
    //             PrintList(&MultiplyResult);
    //             break;
    //         }
            
    //         case 5:
    //         break;
    //         case 0:
    // printf("********************************         Bye~      ***************************************\n");
    //         // exit_all(&head_1);
    //         getchar();
    //         exit(0);  //正常退出
    //         break;
    //         default:
    // printf("****************************输入错误......请重新输入哦***********************************\n");
    //          break;
    //     }
    // printf("****************************  是否继续  Y  or   N?   ************************************\n");
    //      do
    //     {
    //        scanf("%c",&yes_no);
    //     }
	// while(yes_no!='Y'&&yes_no!='y'&&yes_no!='N'&&yes_no!='n');
    // }
    // while(yes_no=='Y'||yes_no=='y');
}









