#include <stdio.h>

void bubble_sort_descending(int arr[], int count)
{
    int i=0, j, temp=0;
    
    for(; i < count; i++)
    {
        for(j=count-1; j>0; j--)
        {
            if(arr[j] > arr[j-1])
            {
                temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = temp;
            }
        }
    }
}

int main(void)
{
    int size, i=0;
    int numArr[10] = {5,4,1,3,7,6,9,2,10,8};

    size = sizeof(numArr) / sizeof(int);
    bubble_sort_descending(numArr, size);

    for(; i<size; i++)
        printf("%d ", numArr[i]);

    return 0;
}
