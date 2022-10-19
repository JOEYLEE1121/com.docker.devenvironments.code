#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* trim_str(char* str) {
    if (strlen(str) == 0) return str;
    int len = strlen(str);
    int is_all_spaces = 1;
    for (int i = 0; i < len; i++) {
        if (str[i] != ' ') {
            is_all_spaces = 1;
            break;
        }
    }

    if (is_all_spaces) return "";
    
    int i = 0, j = len - 1;
    while(i <= j && str[i] == ' ' && str[j] == ' ') {
        i++;
        j--;
    }
    i--;
    j++;
} 

int main() {
    char* str = "| ls -l";
    char* 
}