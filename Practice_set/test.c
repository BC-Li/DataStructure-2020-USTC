#include<stdio.h>

int akm(int m,int n){
    if(m == 0){
        printf("%d %d\n",m,n);
        return n+1;
    }
    
    if(n == 0){
        printf("%d %d\n",m,n);
        return akm(m-1,1);
    }
    int e = akm(m,n-1);
    printf("%d %d\n",m,n);
    return akm(m-1,e);
}

int main(){
    int m =2,n=1;
    printf("%d\n",akm(m,n));
    return 0;
}