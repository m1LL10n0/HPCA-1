#include <stdio.h>

int main(int argc, char const *argv[])
{
    int A[5],B[5],C[5];
    for(int i=0;i<5;i++){
        A[i]=i+1;
        B[i]=6-i;
        C[i]=A[i]*B[i];
    }
    printf("\n");
    for(int i=0;i<5;i++){
        printf("C[%d]=%d ",i,C[i]);
    }
    printf("\n");
    return 0;
}
