#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stddef.h>

#define MAX_SIZE_CMD 8192
#define MAX_SIZE_ARG 8192

char cmd[MAX_SIZE_CMD];

char *argv[MAX_SIZE_ARG];
char *argv2[MAX_SIZE_ARG];
char *argv3[MAX_SIZE_ARG];
char *argv4[MAX_SIZE_ARG];
char *argv5[MAX_SIZE_ARG];

pid_t pid;

int i;
int sizeofargv;

void block_sig();
void get_cmd();
void convert_cmd();
void timeX();
int exitfunc();
int check_pipe();
void without_pipe();
void with_pipe();

int main(){

    while(1){

        get_cmd();

        if(!strcmp(cmd, "")) {
            continue;
        }

        convert_cmd();

        if(!strcmp(cmd, "exit")) {
            if (exitfunc() == 1) {
                break;
            }
            else continue;
        }

        check_pipe();

        if (check_pipe() == 0) {
            without_pipe();
        } else {
            with_pipe();
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
    // add logic here
}

int exitfunc(){
    if (argv[1] != NULL) {
        printf("3230shell: \"exit\" with other arguments!!!\n");
        return 0;
    } else { 
        printf("3230shell: Terminated\n");
        // release all the data and terminate?
        char *terminate[2] = {"ps", NULL};
        execv("/bin/ps", terminate);

        return 1;
    }
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
    sizeofargv = 0;
    ptr = strtok(cmd, " ");
    while(ptr != NULL){
        argv[i] = ptr;
        i++;
        sizeofargv++;
        ptr = strtok(NULL, " ");
    }

    if(!strcmp("&", argv[i-1])){
	    argv[i-1] = NULL;
	    argv[i] = "&";
	    } else{
	        argv[i] = NULL;
	}
}
    //debug argv
    /*
    printf("argv: ");
    for (int i; i<MAX_SIZE_ARG; i++) {
        printf("argv[%d]: %s ", i, argv[i]);
    }
    printf("\n");
    printf("argv2: ");
    for (int i; i<MAX_SIZE_ARG; i++) {
        printf("argv2[%d]: %s ", i, argv2[i]);
    }
    printf("\n");
    printf("argv3: ");
    for (int i; i<MAX_SIZE_ARG; i++) {
        printf("argv3[%d]: %s ", i, argv3[i]);
    }
    printf("\n");
    printf("argv4: ");
    for (int i; i<MAX_SIZE_ARG; i++) {
        printf("argv4[%d]: %s ", i, argv4[i]);
    }
    printf("\n");
    printf("argv5: ");
    for (int i; i<MAX_SIZE_ARG; i++) {
        printf("argv5[%d]: %s ", i, argv5[i]);
    }
    printf("\n");
    */

int check_pipe(){
    int i = 0;
    for (i=0; i < sizeofargv; i++){
        if (!strcmp(argv[i], "|")) {
            printf("pipe detected!!!\n");
            return 1;
        }
    }
    return 0;
}

void without_pipe() {
    pid = fork();
        if(pid < 0){
            fprintf(stderr, "fork() Failed");
            exit(1);
        } else if (pid == 0) {
            execvp(argv[0], argv);
            printf("3230shell: '%s': No such file or directory\n", argv[0]);
            exit(1);
        } else {
            if(NULL == argv[i]) {
                waitpid(pid, NULL, 0);
            }
        }
}

void with_pipe() {
    printf("work with pipe\n");
}