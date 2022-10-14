#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stddef.h>

#define MAX_SIZE_CMD 256
#define MAX_SIZE_ARG 16

char cmd[MAX_SIZE_CMD];
char *argv[MAX_SIZE_ARG];
pid_t pid;
char i;

void block_sig();
void get_cmd();
void convert_cmd();
void timeX();

int main(){

    while(1){

        get_cmd();

        if(!strcmp(cmd, "")) {
            continue;
        }

        if(!strcmp(cmd, "exit")) {
            break;
        }
        printf("cmd before convert: %s\n", cmd);
        convert_cmd();
        printf("cmd after convert: %s\n", cmd);

        pid = fork();
        if(pid < 0){
            fprintf(stderr, "fork() Failed");
            exit(1);
        } else if (pid == 0) {
            execvp(argv[0], argv);
            printf("3230shell: '%s': No such file or directory\n", argv[0]);
            exit(1);
        } else {
            if(NULL == argv[i]) waitpid(pid, NULL, 0);
        }
    }
    return 0;
}

void block_sig(){
    sigset_t block_set;

    sigemptyset(&block_set);

    sigaddset(&block_set, SIGINT);

    sigprocmask(SIG_BLOCK, &block_set, NULL);
}

void timeX(){
    // add exec logic here
}

void get_cmd(){
    printf("$$ 3230shell ## ");
    block_sig();
    fgets(cmd, MAX_SIZE_CMD, stdin);
	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n'))
        	cmd[strlen (cmd) - 1] = '\0';
}

void convert_cmd(){

    char *ptr;
    i = 0;
    ptr = strtok(cmd, " ");
    while(ptr != NULL){
        argv[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
    }

    if(!strcmp("&", argv[i-1])){
	    argv[i-1] = NULL;
	    argv[i] = "&";
	    } else{
	        argv[i] = NULL;
	}

    printf("argv[0]: %s\n",argv[0]);

    //add pipe here later

}