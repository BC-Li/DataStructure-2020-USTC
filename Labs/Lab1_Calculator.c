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
    int note;
    struct LinkedList *next; 
}SqList;

//  import functions

int PrintList(SqList *A);
int DetectEmptyList(SqList *A);
int ListFree(SqList *L);

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
    
    // use 'note' to mark the first factor.
    scanf("%d",&coefficient);
    scanf("%d",&index_filler);
    p = (SqList *)malloc(sizeof(SqList));
    p->data = coefficient;
    p->index = index_filler;
    p->note = 1;
    end->next = p;
    end = p;

    while((scanf("%d",&coefficient)!= EOF)&&(scanf("%d",&index_filler)!= EOF)){
        p = (SqList *)malloc(sizeof(SqList));
        p->data = coefficient;
        p->index = index_filler;
        p->note = 0;
        end->next = p;
        end = p;
    }
    end->next = NULL;
    return OK;
}

int OutputListInit(SqList *L){  // Init a linkedlist which is used for storing the outputs.
    SqList *p = NULL, *end = NULL;
    end = L;
    L = (SqList *)malloc(sizeof(SqList));
    if(!L){
        return ERROR;
    }
    L->next = NULL;
    p = (SqList *)malloc(sizeof(SqList));
    p->data = 0;
    p->index = 0;
    p->note = 0;
    end->next = p;
    end = p;
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
    p->note = 0;

    end = L;
    while(end->next!=NULL){
        end = end->next;
    }
    end->next = p;
    return OK;
}

// Calculate functions for LinkedList 

int ListAddition(SqList *A, SqList *B, SqList *Addition){
    SqList *searcher_A = A->next,*searcher_B = B->next;
    SqList * Traveler;
    int n = 0;
    float e = 0;
    Addition->next = NULL;
    // the first factor
    
    // if((searcher_A->next!=NULL)&&(searcher_B->next!=NULL)){
    //     if(searcher_A->index == n){
    //         e = e+searcher_A->data;
    //         searcher_A = searcher_A ->next;
    //     }

    //     if(searcher_B->index == n){
    //         e = e+searcher_B->data;
    //         searcher_B = searcher_B ->next;
    //     }

    //     ListInsert(e,n,Addition);
    //     e = 0;
    //     n++;
        
    // }
    
    while((searcher_A!=NULL)||(searcher_B!=NULL)){

        if(searcher_A!=NULL && searcher_A->index == n){
            e = e+searcher_A->data;
            searcher_A = searcher_A ->next;
        }

        if(searcher_B!=NULL && searcher_B->index == n){
            e = e+searcher_B->data;
            searcher_B = searcher_B ->next;
        }

        ListInsert(e,n,Addition);
        e = 0;
        n++;
    }

    Traveler = Addition->next; // make the first factor of Addition's note 1
    while(Traveler->next != NULL ){
        Traveler = Traveler->next;
    }
    Traveler->note = 1;

    return OK;
}

int ListSubtraction(SqList *A,SqList *B, SqList *Substraction){    // A-B
    SqList *searcher_A = A,*searcher_B = B;
    int n = 0, e = 0;
    Substraction->next = NULL;
    while((searcher_A->next!=NULL)&&(searcher_B->next!=NULL)){

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

int DetectEmptyList(SqList *A){
    SqList *Searcher;
    Searcher = A;
    if(Searcher == NULL){
        return -1;
    }
    Searcher = Searcher->next;
    while(Searcher!=NULL){
        if(Searcher->data > 0.0000000001){
            return 1;
        }
        Searcher = Searcher -> next;
    }
    return -1;
}

int SearchList(float x,float y, SqList *L){
    SqList *p = L;
    while(p!=NULL){
        if(y == p->index){
            p->data = p->data + x;
            return 1;
        }
        p = p->next;
    }
    return 0;
}

int CopyList(SqList *A, SqList *B){
    // ListFree(B);

    return OK;
}

int ListMultiply(SqList *A, SqList *B, SqList *Output, SqList *MultiplyTemp){
    // OutputListInit(MultiplyTemp);
    // OutputListInit(Output);
    SqList Temp_1;
    MultiplyTemp->next = NULL;
    Temp_1.data = 0;
    Temp_1.index = 0;
    Temp_1.note = 0; //test
    Temp_1.next = NULL;
    SqList *searcher_A = A->next, *searcher_B = B->next;
    while(searcher_A!=NULL){
        while(searcher_B!=NULL){
            int MultiplyedData = searcher_A->data * searcher_B->data;
            int MultipliedIndex = searcher_A->index + searcher_B->index;

            if(SearchList(MultiplyedData,MultipliedIndex,MultiplyTemp)==0){
                ListInsert(MultiplyedData,MultipliedIndex,MultiplyTemp);
            }
            searcher_B = searcher_B->next;
        }
        ListAddition(MultiplyTemp,Output,&Temp_1);
        searcher_A = searcher_A->next;
        searcher_B = B->next;
        // CopyList(&Temp_1,Output);
        Output->next = NULL;
        SqList *p;
        for(p = Temp_1.next;p!=NULL;p = p->next){
            ListInsert(p->data,p->index,Output);
        }
        Temp_1.next = NULL;
        MultiplyTemp->next = NULL;
    }
    SqList *Traveler = Output->next; // make the first factor of Multiply's note 1
    while(Traveler->next != NULL ){
        Traveler = Traveler->next;
    }
    Traveler->note = 1;

    PrintList(Output);
    return OK;
} 

//  Standard Output and Other Features

int PrintList(SqList *A){       // reverse print using recursive func
    // if(DetectEmptyList(A) == -1){
    //     printf("0,this is an empty list");
    //     return OK;
    // }
    // else{
        
    if(A!=NULL){
    PrintList(A->next);
    if(A == NULL){
        return ERROR;   // the list is empty or something else
    }

    // Head of the polynomial, add some special judgements 
    if(A->data > 0 && (fabsf(A->data) > 0.000001) && A->note == 1){  // note = 1 means the first factor in the polynomial
        if(A->index == 0){
            printf("%f",A->data);
        }
       printf("%fx^%d",A->data,A->index);
    }

    if(A->data > 0 && (fabsf(A->data) > 0.000001) && A->note == 0){  // avoid invalid judgement of zero regarding to float type  
        if(A->index == 0){
            printf("+%f",A->data);
        }
        printf("+%fx^%d",A->data,A->index);
    }

    if(A->data < 0){     
        if(A->index == 0){
            printf("%f",A->data);
        }
        printf("%fx^%d",A->data,A->index);
    }
    

    return OK;
    }
    else{
        return ERROR;
    }
    
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
    SqList *p = L->next;
    while(p!=NULL){
        SqList *temp = p;
        p = p->next;
        free(temp);
    }
    return OK;
}

void welcome(){
	printf ("\n");
	printf ("\n");
	printf ("\n");
    printf("***************  Unary sparse polynomial calculator By Bocheng Li*************************\n");
    printf("***************   View https://github.com/BC-Li for more info.   *************************\n");
    printf("\n");
    printf("\n");
    printf("********************************    1.INIT         ***************************************\n");
    printf("********************************    2.Addition     ***************************************\n");
    printf("********************************    3.Substraction ***************************************\n");
    printf("********************************    4.Multiply     ***************************************\n");
    printf("********************************    5.Derivative   ***************************************\n");
    printf("********************************    0.EXIT         ***************************************\n\n\n");
 
}

int main() 
{ 
    // SqList A,B,Addition;
    // ListInit(&A);
    // ListInit(&B);
    // OutputListInit(&Addition);
    // ListAddition(&A,&B,&Addition);
    // PrintList(&Addition);
    // return 0;
    char  choice[100];
    int choice_1;
    char  yes_no;
    
    
    do
    {
        welcome();
    printf("*******************************    Input your command  0-5:  *****************************\n");
        scanf("%s",choice);
        choice_1=choice[0]-48;
        printf("\n");
        switch(choice_1)
       {
            case 1:{  //INIT
                SqList A, B;
                ListInit(&A);
                ListInit(&B);
                break;
            }
            case 2:{
                SqList A, B;
                ListInit(&A);
                ListInit(&B);
                SqList AdditionResult;
                ListAddition(&A,&B,&AdditionResult);
                PrintList(&AdditionResult);
                ListFree(&A);
                ListFree(&B);
                break;
            }
            case 3:{
                SqList A, B;
                ListInit(&A);
                ListInit(&B);
                SqList SubtractionResult;
                ListSubtraction(&A,&B,&SubtractionResult);
                PrintList(&SubtractionResult);
                ListFree(&A);
                ListFree(&B);
                break;
            }
            case 4:{
            SqList A,B,Addition,Multiply;
            ListInit(&A);
            ListInit(&B);
            OutputListInit(&Addition);
            OutputListInit(&Multiply);
            ListMultiply(&A,&B,&Addition,&Multiply);
            ListFree(&A);
            ListFree(&B);
            return 0;
                break;
            }
            
            case 5:{
                SqList A;
                ListInit(&A); 
                DerivativeFunction(&A);
                ListFree(&A);
                break;
            }
            
            case 0:
    printf("********************************         Bye~      ***************************************\n");
            // exit_all(&head_1);
            getchar();
            exit(0);  //正常退�?
            break;
            default:
    printf("****************************输入错误......请重新输入哦***********************************\n");
             break;
        }
    printf("****************************  是否继续  Y  or   N?   ************************************\n");
         do
        {
           scanf("%c",&yes_no);
        }
	while(yes_no!='Y'&&yes_no!='y'&&yes_no!='N'&&yes_no!='n');
    }
    while(yes_no=='Y'||yes_no=='y');
}









