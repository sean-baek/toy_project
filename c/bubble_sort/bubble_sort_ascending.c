#include <stdio.h>

void bubble_sort_ascending(int arr[], int count)
{
    int temp=0;
    int i=0, j;

    for(; i<count; i++)
    {
        for(j=0; j<count-1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(void)
{
    int i=0, size=0;
    int numArr[10] = {8,4,2,5,3,7,10,1,6,9};

    size = sizeof(numArr) / sizeof(int);
    bubble_sort_ascending(numArr, size);

    for(; i < size; i++)
    {
        printf("%d ", numArr[i]);
    }

    printf("\n");

    return 0;
}
