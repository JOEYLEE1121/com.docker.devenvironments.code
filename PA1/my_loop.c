#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(){
    int sec = 3;
    int cnt = 0;

    while(cnt < sec) {
        printf("Hello!\n");
        sleep(3);
        cnt++;
    }
}