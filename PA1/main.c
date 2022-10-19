#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <stddef.h>

#define MAX_SIZE_CMD 8192
#define MAX_SIZE_ARG 8192

char cmd[MAX_SIZE_CMD];

char *argv[MAX_SIZE_ARG];

int sizeofargv;
int i;

void get_cmd(){
    printf("$$ 3230shell ## ");
    //block_sig();
    fgets(cmd, MAX_SIZE_CMD, stdin);
	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n'))
        	cmd[strlen (cmd) - 1] = '\0';
}
"ab c df"
"ls -l"
"grep sh"
void convert_cmd(){

    char *ptr;
    i = 0;
    sizeofargv = 0;
    ptr = strtok(cmd, " ");
    while(ptr != NULL){
        argv[i] = ptr;
        i++;
        sizeofargv++;
        ptr = strtok(NULL, " ");
    }

    if(!strcmp("&", argv[i-1])){
        
	    } else{
	        argv[i] = NULL;
	}

    char one;
    strcpy(one, ".");

    if(!strcmp(&argv[0][0],&one)){
        printf("3230shell: '%s': Permission denied\n", argv[0]);
    }
    printf("%c\n",one);
    

}

int main(){
    while(1){
        get_cmd();
        convert_cmd();
    }
}