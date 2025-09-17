#include <stdio.h>

void printArray(int *arr, int len)
{
  for (int i = 0; i < len; i++)
  {
    printf("%d  ", arr[i]);
  }
  printf("\n");
}
void bubbleSort(int *arr, int len)
{
  for (int i = 0; i < len - 1; i++)
  {
    printf("------------------\n");
    int swapped = 0;
    for (int j = 0; j < len - i - 1; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
        swapped = 1;
        printf("i=%d j=%d >> ", i, j);

        printArray(arr, len);
      }
    }
    if (!swapped)
    {
      break;
    }
  }
}

int main(int argc, char const *argv[])
{
  /* code */
  // 1.创建数组
  int arr[10] = {4, 3, 6, 9, 1, 2, 10, 8, 7, 5};
  int len = sizeof(arr) / sizeof(arr[0]);
  printArray(arr, len);
  // 排序
  bubbleSort(arr, len);
  return 0;
}
