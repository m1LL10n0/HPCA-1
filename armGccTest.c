#include <stdio.h>

int main(int argc, char const *argv[])
{
    int count = 1000;
    int A[count],B[count],C[count];
    for(int i=0;i<count;i++){
        A[i]=i+1;
        B[i]=count+1-i;
        C[i]=A[i]*B[i];
    }
    printf("\n");
    for(int i=0;i<count;i++){
        printf("C[%d]=%d ",i,C[i]);
    }
    printf("\n");
    return 0;
}
