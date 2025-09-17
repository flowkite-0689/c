#include <stdio.h>
#include <string.h>

int main() {
    char arr[] = "apple tree";
    int j = 0;

    int len = strlen(arr); 

    for (int i = 0; i < len; i++) {
        char c = arr[i];
        if (c != ' ') {
            arr[j++] = arr[i]; // 非空格字符前移
        }
    }
    arr[j] = '\0'; 
    
    puts(arr); 
    return 0;
}