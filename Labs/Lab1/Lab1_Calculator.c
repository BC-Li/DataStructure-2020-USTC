// This is the lab 1: Unary sparse polynomial calculator for DataStructure 2020 Fall by Bocheng Li.
// View my GitHub: https://github.com/BC-Li for more info.

// include libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OK 1
#define ERROR 0
#define LISTINITSIZE 100
#define Elemtype SqList *

typedef struct LinkedList
{
    float data;
    int index;
    int note;
    struct LinkedList *next;
} SqList;

typedef struct StoreList
{
    Elemtype *elem;
    int length;
    int Listsize;
} StoreList;

//  import functions

int PrintList(SqList *A);
int DetectEmptyList(SqList *A);
int ListFree(SqList *L);
int SearchList(float x, float y, SqList *L);

// init Storelist

int InitStoreList(StoreList *L)
{
    L->elem = (Elemtype *)malloc((LISTINITSIZE) * sizeof(SqList *));
    if (!L->elem)
        exit(OVERFLOW);
    L->length = 0;
    L->Listsize = LISTINITSIZE;
    return OK;
}

int ListInsertStoreList(StoreList *L, int i, Elemtype e)
{
    if (L->length >= L->Listsize)
    {
        Elemtype *newbase;
        newbase = (Elemtype *)realloc(L->elem, (LISTINITSIZE + 10) * sizeof(Elemtype));
        if (!newbase)
            exit(OVERFLOW);
        L->elem = newbase;
        L->Listsize = L->Listsize + 10;
    }
    Elemtype *q = &(L->elem[i]);
    for (Elemtype *p = &(L->elem[L->length - 1]); p >= q; --p)
    {
        *(p + 1) = *p;
    }
    *q = e; // ???
    ++L->length;
    return OK;
}

int ListDeleteStoreList(StoreList *L, int i)
{
    if ((i < 1) || (i > L->length))
        return ERROR;
    Elemtype *p = &(L->elem[i - 1]);
    Elemtype *q = L->elem + L->length - 1;
    for (++p; p <= q; ++p)
    {
        *(p - 1) = *p;
    }
    --L->length;
    return OK;
}

void ListSort(SqList *L)
{
    SqList *p, *q;
    for (p = L->next; p != NULL; p = p->next)
    {
        for (q = p->next; q != NULL; q = q->next)
        {
            if ((p->index) > (q->index))
            {
                int s = q->index;
                q->index = p->index;
                p->index = s;
                float x = q->data;
                q->data = p->data;
                p->data = x;
            }
        }
    }
}

SqList *ListInit()
{
    SqList *L;
    SqList *p = NULL, *end = NULL;
    int coefficient;
    int index_filler;
    int counter;
    L = (SqList *)malloc(sizeof(SqList));
    end = L;
    if (!L)
    {
        return ERROR;
    }
    L->data = 0;
    L->index = 0;
    L->note = 0;
    L->next = NULL;
    printf("How many terms do you want to put in this Polynomial?\n");
    scanf("%d", &counter);
    printf("Please input the coefficient and index. following order like '1 2'. press ctrl+z to stop.\nNote: Invalid input may cause unexpected error.\n");
    // use 'note' to mark the first factor.
    scanf("%d", &coefficient);
    scanf("%d", &index_filler);
    p = (SqList *)malloc(sizeof(SqList));
    p->data = coefficient;
    p->index = index_filler;
    p->note = 0;
    end->next = p;
    end = p;
    end->next = NULL;

    // while((scanf("%d",&coefficient)!= EOF)&&(scanf("%d",&index_filler)!= EOF)){
    for (int i = 2; i <= counter; i++)
    {
        scanf("%d", &coefficient);
        scanf("%d", &index_filler);
        if (SearchList(coefficient, index_filler, L) == 1)
        { //  same index return 1
        }
        else
        {
            p = (SqList *)malloc(sizeof(SqList));
            p->data = coefficient;
            p->index = index_filler;
            p->note = 0;
            end->next = p;
            end = p;
            end->next = NULL;
        }
    }
    end->note = 1;
    ListSort(L);
    return L;
}
SqList *ListCopy(SqList *ListForCP)
{

    SqList *Searcher = ListForCP->next;
    SqList *L;
    SqList *p = NULL, *end = NULL;
    L = (SqList *)malloc(sizeof(SqList));
    end = L;
    if (!L)
    {
        return ERROR;
    }
    L->data = 0;
    L->index = 0;
    L->note = 0;
    L->next = NULL;
    p = (SqList *)malloc(sizeof(SqList));
    p->data = Searcher->data;
    p->index = Searcher->index;
    p->note = Searcher->note;
    end->next = p;
    end = p;
    end->next = NULL;
    Searcher = Searcher->next;

    // while((scanf("%d",&coefficient)!= EOF)&&(scanf("%d",&index_filler)!= EOF)){
    while (Searcher != NULL)
    {
        p = (SqList *)malloc(sizeof(SqList));
        p->data = Searcher->data;
        p->index = Searcher->index;
        p->note = Searcher->note;
        end->next = p;
        end = p;
        end->next = NULL;
        Searcher = Searcher->next;
    }
    end->note = 1;
    return L;
}

int OutputListInit(SqList *L)
{ // Init a linkedlist which is used for storing the outputs.
    SqList *p = NULL, *end = NULL;
    end = L;
    if (!L)
    {
        return ERROR;
    }
    L->data = 0;
    L->index = 0;
    L->note = 0;
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

int ListInsert(float e, int n, SqList *L)
{
    SqList *p, *end; // insert A+B to C

    p = (SqList *)malloc(sizeof(SqList));
    if (!p)
    {
        return ERROR;
    }

    p->next = NULL;
    p->data = e;
    p->index = n;
    p->note = 0;

    end = L;
    while (end->next != NULL)
    {
        end = end->next;
    }
    end->next = p;
    return OK;
}

int ListAddition(SqList *A, SqList *B, SqList *Addition)
{
    SqList *searcher_A = A->next, *searcher_B = B->next;
    SqList *Traveler;
    // int n = -1000000;
    // float e = 0;
    Addition->next = NULL;

    while ((searcher_A != NULL) && (searcher_B != NULL))
    {
        if (searcher_A->index < searcher_B->index)
        {
            ListInsert(searcher_A->data, searcher_A->index, Addition);
            searcher_A = searcher_A->next;
            continue;
        }
        if (searcher_A->index == searcher_B->index)
        {
            ListInsert(searcher_A->data + searcher_B->data, searcher_A->index, Addition);
            searcher_A = searcher_A->next;
            searcher_B = searcher_B->next;
            continue;
        }
        if (searcher_A->index > searcher_B->index)
        {
            ListInsert(searcher_B->data, searcher_B->index, Addition);
            searcher_B = searcher_B->next;
            continue;
        }
    }
    while (searcher_A != NULL)
    {
        ListInsert(searcher_A->data, searcher_A->index, Addition);
        searcher_A = searcher_A->next;
    }
    while (searcher_B != NULL)
    {
        ListInsert(searcher_B->data, searcher_B->index, Addition);
        searcher_B = searcher_B->next;
    }

    Traveler = Addition->next; // make the first factor of Addition's note 1
    while (Traveler->next != NULL)
    {
        Traveler->note = 0;
        Traveler = Traveler->next;
    }
    Traveler->note = 1;

    return OK;
}
int ListSubtraction(SqList *A, SqList *B, SqList *Substraction)
{
    SqList *searcher_A = A->next, *searcher_B = B->next;
    SqList *Traveler;
    // int n = -1000000;
    // float e = 0;
    Substraction->next = NULL;

    while ((searcher_A != NULL) && (searcher_B != NULL))
    {
        if (searcher_A->index < searcher_B->index)
        {
            ListInsert(searcher_A->data, searcher_A->index, Substraction);
            searcher_A = searcher_A->next;
            continue;
        }
        if (searcher_A->index == searcher_B->index)
        {
            ListInsert(searcher_A->data - searcher_B->data, searcher_A->index, Substraction);
            searcher_A = searcher_A->next;
            searcher_B = searcher_B->next;
            continue;
        }
        if (searcher_A->index > searcher_B->index)
        {
            ListInsert(-searcher_B->data, searcher_B->index, Substraction);
            searcher_B = searcher_B->next;
            continue;
        }
    }
    while (searcher_A != NULL)
    {
        ListInsert(searcher_A->data, searcher_A->index, Substraction);
        searcher_A = searcher_A->next;
    }
    while (searcher_B != NULL)
    {
        ListInsert(-searcher_B->data, searcher_B->index, Substraction);
        searcher_B = searcher_B->next;
    }

    Traveler = Substraction->next; // make the first factor of Addition's note 1
    while (Traveler->next != NULL)
    {
        Traveler->note = 0;
        Traveler = Traveler->next;
    }
    Traveler->note = 1;

    return OK;
}
int DetectEmptyList(SqList *A)
{
    SqList *Searcher;
    Searcher = A;
    if (Searcher == NULL)
    {
        return -1;
    }
    Searcher = Searcher->next;
    while (Searcher != NULL)
    {
        if (fabsf(Searcher->data) > 0.0000000001)
        {
            return 1;
        }
        Searcher = Searcher->next;
    }
    return -1;
}

int SearchList(float x, float y, SqList *L)
{
    SqList *p = L->next;
    while (p != NULL)
    {
        if (y == p->index)
        {
            p->data = p->data + x;
            return 1; //  same index return 1
        }
        p = p->next;
    }
    return 0;
}


int ListMultiply(SqList *A, SqList *B, SqList *Output, SqList *MultiplyTemp)
{
    // OutputListInit(MultiplyTemp);
    // OutputListInit(Output);
    SqList Temp_1;
    MultiplyTemp->next = NULL;
    Output->next = NULL;
    Temp_1.data = 0;
    Temp_1.index = 0;
    Temp_1.note = 0; //test
    Temp_1.next = NULL;
    SqList *searcher_A = A->next, *searcher_B = B->next;
    while (searcher_A != NULL)
    {
        while (searcher_B != NULL)
        {
            int MultiplyedData = searcher_A->data * searcher_B->data;
            int MultipliedIndex = searcher_A->index + searcher_B->index;

            if (SearchList(MultiplyedData, MultipliedIndex, MultiplyTemp) == 0)
            {
                ListInsert(MultiplyedData, MultipliedIndex, MultiplyTemp);
            }
            searcher_B = searcher_B->next;
        }
        ListAddition(MultiplyTemp, Output, &Temp_1);
        searcher_A = searcher_A->next;
        searcher_B = B->next;
        // CopyList(&Temp_1,Output);
        Output->next = NULL;
        SqList *p;
        for (p = Temp_1.next; p != NULL; p = p->next)
        {
            ListInsert(p->data, p->index, Output);
        }
        Temp_1.next = NULL;
        MultiplyTemp->next = NULL;
    }
    SqList *Traveler = Output->next; // make the first factor of Multiply's note 1
    while (Traveler->next != NULL)
    {
        Traveler->note = 0;
        Traveler = Traveler->next;
    }
    Traveler->note = 1;

    PrintList(Output);
    return OK;
}

//  Standard Output and Other Features

int PrintList(SqList *A)
{ 
    // reverse print using recursive func
    // if(DetectEmptyList(A) == -1){
    //     printf("0,this is an empty list");
    //     return OK;
    // }
    // else{

    if (A != NULL)
    {
        PrintList(A->next);
        if (A == NULL)
        {
            return ERROR; // the list is empty or something else
        }

        // Head of the polynomial, add some special judgements
        if (A->data > 0 && (fabsf(A->data) > 0.000001) && A->note == 1)
        { // note = 1 means the first factor in the polynomial
            if (A->index == 0)
            {
                printf("%f", A->data);
            }
            else if (A->index == 1)
            {
                if (A->data == 1)
                {
                    printf("x");
                }
                else
                {
                    printf("%fx", A->data);
                }
            }
            else if (A->data == 1)
            {
                printf("x^%d", A->index);
            }
            else
            {
                printf("%fx^%d", A->data, A->index);
            }
        }

        if (A->data > 0 && (fabsf(A->data) > 0.000001) && A->note == 0)
        { // avoid invalid judgement of zero regarding to float type
            if (A->index == 0)
            {
                printf("+%f", A->data);
            }
            else if (A->index == 1)
            {
                if (A->data == 1)
                {
                    printf("+x");
                }
                else
                {
                    printf("+%fx", A->data);
                }
            }
            else if (A->data == 1)
            {
                printf("+x^%d", A->index);
            }
            else
            {
                printf("+%fx^%d", A->data, A->index);
            }
        }

        if (A->data < 0 && (fabsf(A->data) > 0.00000000001))
        {
            if (A->index == 0)
            {
                printf("%f", A->data);
            }
            else if (A->index == 1)
            {
                if (A->data == -1)
                {
                    printf("-x");
                }
                else
                    printf("%fx", A->data);
            }
            else if (A->data == -1)
            {
                printf("-x^%d", A->index);
            }
            else
            {
                printf("%fx^%d", A->data, A->index);
            }
        }

        return OK;
    }
    else
    {
        return ERROR;
    }
}
//  Deprecated Function
// int DerivativeFunction(SqList *A){   // F'(x)
//     SqList *Traveler = A;
//     if(Traveler->next == NULL){
//         return ERROR;   // the list is empty or something else
//     }
//     while(Traveler != NULL){
//         printf("%fx^%d",Traveler->data*Traveler->index,Traveler->index-1);
//     }
//     return OK;
// }

float CalculateValue(SqList *A, float x)
{ // Calculate the value of f(x) (given x)
    float Value = 0;
    SqList *Traveler = A->next;
    if (Traveler == NULL)
    {
        return ERROR; // the list is empty or something else
    }
    while (Traveler != NULL)
    {
        Value = Value + Traveler->data * (pow(x, Traveler->index));
        Traveler = Traveler->next;
    }
    return Value;
}

int ListFree(SqList *L)
{
    SqList *p = L->next;
    while (p != NULL)
    {
        SqList *temp = p;
        p = p->next;
        free(temp);
    }
    return OK;
}

void welcome()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("******************************************************************************************\n");
    printf("*****************Unary sparse polynomial calculator By Bocheng Li*************************\n");
    printf("*******************View https://github.com/BC-Li for more info.***************************\n");
    printf("************************************   MENU    *******************************************\n");
    printf("********************************    1.INIT         ***************************************\n");
    printf("********************************    2.Addition     ***************************************\n");
    printf("********************************    3.Substraction ***************************************\n");
    printf("********************************    4.Multiply     ***************************************\n");
    printf("******************************** 5.Calculate Value ***************************************\n");
    printf("********************************    6.Print        ***************************************\n");
    printf("********************************    7.Free         ***************************************\n");
    printf("********************************    8.Print all    ***************************************\n");
    printf("********************************    0.EXIT         ***************************************\n");
    printf("******************************************************************************************\n\n\n");
}

int main()
{
    char choice[100];
    int choice_1;
    char yes_no;
    int PolynomialNumbers;
    StoreList Polynomial;
    InitStoreList(&Polynomial);
    // PrintList(Polynomial.elem[1]);

    do
    {
        welcome();
        printf("*******************************    Input your command  0-5:  *****************************\n");
        scanf("%s", choice);
        choice_1 = choice[0] - 48;
        printf("\n");
        switch (choice_1)
        {
        case 1:
        { //INIT
            printf("How many polynomials do you want to init?\n");
            scanf("%d", &PolynomialNumbers);
            int PolynomialTempLength = Polynomial.length;
            for (int i = 0; i < PolynomialNumbers; i++)
            {
                ListInsertStoreList(&Polynomial, PolynomialTempLength + i, ListInit());
            }
            break;
        }
        case 2:
        {
            int A, B;
            printf("Which 2 Polynomials would you want to add?\n");
            scanf("%d %d", &A, &B);
            SqList AdditionResult;
            ListAddition(Polynomial.elem[A - 1], Polynomial.elem[B - 1], &AdditionResult);
            ListInsertStoreList(&Polynomial, Polynomial.length, &AdditionResult);
            PrintList(&AdditionResult);
            break;
        }
        case 3:
        {
            int A, B;
            printf("Which 2 Polynomials would you want to substract? A-B\n");
            scanf("%d %d", &A, &B);
            SqList SubtractionResult;
            ListSubtraction(Polynomial.elem[A - 1], Polynomial.elem[B - 1], &SubtractionResult);
            ListInsertStoreList(&Polynomial, Polynomial.length, &SubtractionResult);
            PrintList(&SubtractionResult);
            break;
        }
        case 4:
        {
            int A, B;
            printf("Which 2 Polynomials would you want to Multiply?\n");
            scanf("%d %d", &A, &B);
            SqList MultiplyResult;
            SqList MultiplyTemp;
            OutputListInit(&MultiplyTemp);
            OutputListInit(&MultiplyResult);
            ListMultiply(Polynomial.elem[A - 1], Polynomial.elem[B - 1], &MultiplyResult, &MultiplyTemp);
            ListInsertStoreList(&Polynomial, Polynomial.length, ListCopy(&MultiplyResult));
            ListFree(&MultiplyResult);
            ListFree(&MultiplyTemp);
            break;
        }

            // case 5:{        //5.Derivative
            //     int DerivativeNumber;
            //     printf("Which Polynomial would you want to output Derivative ?\n");
            //     scanf("%d",&DerivativeNumber);
            //     DerivativeFunction(Polynomial.elem[DerivativeNumber-1]);
            //     break;
            // }

        case 6:
        {
            int PrintListNumber;
            printf("Which Polynomial would you want to print?\n");
            scanf("%d", &PrintListNumber);
            PrintList(Polynomial.elem[PrintListNumber - 1]);
            break;
        }

        case 7:
        {
            int FreeListNumber;
            printf("Which Polynomial would you want to free?\n");
            scanf("%d", &FreeListNumber);
            ListFree(Polynomial.elem[FreeListNumber - 1]);
            // See if it is useful: SqList * Mover = NULL;
            for (int i = FreeListNumber; i <= Polynomial.length; i++)
            {
                Polynomial.elem[i - 1] = Polynomial.elem[i];
            }
            Polynomial.length--;
            break;
        }

        case 8:
        {
            printf("Print all the polynomials !!!\n");
            for (int i = 0; i < Polynomial.length; i++)
            {
                printf("The No.%d polynomial is: ", i + 1);
                PrintList(Polynomial.elem[i]);
                printf("\n");
            }
            break;
        }
        case 5:
        {
            printf("Which Polynomial would you want to calculate?\n");
            int CalculateListNumber;
            float x;
            scanf("%d", &CalculateListNumber);
            printf("Input x please.\n");
            scanf("%f", &x);
            printf("the value is %f", CalculateValue(Polynomial.elem[CalculateListNumber - 1], x));
            break;
        }
        case 0:
            printf("********************************         Bye~      ***************************************\n");
            getchar();
            exit(0);
            break;
        default:
            printf("****************************EMM you seem to enter the wrong letters qwq*******************\n");
            break;
        }
        printf("\n****************************  Do you want to proceed? ************************************\n");
        do
        {
            scanf("%c", &yes_no);
        } while (yes_no != 'Y' && yes_no != 'y' && yes_no != 'N' && yes_no != 'n');

        //  system("CLS");
    } while (yes_no == 'Y' || yes_no == 'y');
}
