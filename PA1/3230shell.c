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
int exitfunc();

int main(){

    while(1){

        get_cmd();

        if(!strcmp(cmd, "")) {
            continue;
        }

        // printf("cmd before convert: %s\n", cmd);
        convert_cmd();
        // printf("cmd after convert: %s\n", cmd);

        if(!strcmp(cmd, "exit")) {
            if (exitfunc() == 1) {
                break;
            }
            else continue;
        }

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

int exitfunc(){
    if (argv[1] != NULL) {
        printf("3230shell: \"exit\" with other arguments!!!\n");
        return 0;
    } else {
        printf("3230shell: Terminated");
        // realase all resources and terminate. show the 
        return 1;
    }
    // add exit logic here
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

    //add pipe here later

}