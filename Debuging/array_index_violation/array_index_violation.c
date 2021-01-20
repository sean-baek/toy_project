#include <stdio.h>

int main(void)
{
    int i, j;
    int arrA[2][4] = {{1,2,3,4}, {5,6,7,8}};
    int arrB[4][2]={0, };
    

    int row = sizeof(arrA[0]) / sizeof(int);

    int col = sizeof(arrA) / sizeof(arrA[0]);

    for(i=0; i<col; i++)
    {
        for(j=0; j<row; j++)
        {
            arrB[j][i] = arrA[i][j];
        }
        printf("%d", arrB[i][j]);
        printf("\n");
    }
}
