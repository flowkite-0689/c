#include <stdio.h>

int main(void)
{
    char buf_1[] = {'a', 'b', 'c', 'd'};
    char buf_2[] = {"abcd"};
    
    printf("buf_1:%s, sizeof(buf_1):%d\n", buf_1, sizeof(buf_1));
    printf("buf_2:%s, sizeof(buf_2):%d\n", buf_2, sizeof(buf_2));
}