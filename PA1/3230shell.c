/*
Sutdent Info:
    Name: Chang Jun Lee
    UID: 3035729629

Reference list:
    1 pipe: https://www.youtube.com/watch?v=Mqb2dVRe0uo&t=566s
    SIGINT: https://stackoverflow.com/questions/54352563/block-sigint-from-terminating-program
    strtok: https://www.youtube.com/watch?v=zDjLADJGXFs
*/

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

// user input will be stored here
char *argv[MAX_SIZE_ARG];

// if pipe detected, arguments will be distributed to these arrays
char *argv1[MAX_SIZE_ARG];
char *argv2[MAX_SIZE_ARG];
char *argv3[MAX_SIZE_ARG];
char *argv4[MAX_SIZE_ARG];
char *argv5[MAX_SIZE_ARG];

char *timeXtmp[MAX_SIZE_ARG];

pid_t pid;

int i;
int sizeofargv;
int funcerr;
int pipecount;

void block_sig();
void get_cmd();
void convert_cmd();
int timeXfunc();
int exitfunc();
int check_pipe();
void without_pipe();
int div_arg();
int with_pipe();

int main(){
    funcerr = 0;
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

        if(!strcmp(cmd, "timeX")){
            if (timeXfunc() == 1) {
                break;
            } 
            else continue;
        }

        if (check_pipe() == 0) {
            without_pipe();
        } else {
            // divide arguments into separate arrays
            div_arg();
            with_pipe();
        }
    }
    return 0;
}

// blocks SIGINT
void block_sig(){
    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigprocmask(SIG_BLOCK, &block_set, NULL);
}

int exitfunc(){
    // error handle for exit with other arguments
    if (argv[1] != NULL) {
        printf("3230shell: \"exit\" with other arguments!!!\n");
        return 0;
    } else { 
        // exit
        printf("3230shell: Terminated\n");
        exit(0);
    }
}


int timeXfunc(){
    if (!strcmp(argv[sizeofargv-1],"&")){
        printf("3230shell: \"timeX\" cannot be run in background mode\n");
        funcerr++;
        return 0;
    } else if (sizeofargv == 1 && !strcmp(argv[sizeofargv-1],"timeX")){
        printf("\"timeX\" cannot be a standalone command\n");
        funcerr++;
        return 0;
    } else if (pipecount == 0) {
        int t_pid = fork();
        if (t_pid < 0) {
            fprintf(stderr, "timeX failed\n");
            exit(-1);
        } else if (t_pid == 0){
            for (int i=0; i<sizeofargv; i++){
                timeXtmp[i] = argv[i+1];
                if (i+1 == sizeofargv) {
                    break;
                }
            }
            execvp(timeXtmp[0], timeXtmp);
            exit(0);
        } else {
            int status;
            struct rusage rusage;
            int ret = wait4(t_pid, &status, 0, &rusage);
            printf("(PID)%d (CMD)%s (user)%.3f s (sys)%.3f s \n",t_pid, argv[1], rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / 1000000.0, rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / 1000000.0);
        }
    } else if (pipecount == 1) {
        int t_pid = fork();
        if (t_pid < 0) {
            fprintf(stderr, "timeX failed\n");
            exit(-1);
        } else if (t_pid == 0){
            for (int i=0; i<sizeofargv; i++){
                timeXtmp[i] = argv[i+1];
                if (i+1 == sizeofargv) {
                    break;
                }
            }
            execvp(timeXtmp[0], timeXtmp);
            exit(0);
        } else {
            int status;
            struct rusage rusage;
            int ret = wait4(t_pid, &status, 0, &rusage);
            printf("(PID)%d (CMD)%s (user)%.3f s (sys)%.3f s \n",t_pid, argv[1], rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / 1000000.0, rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / 1000000.0);
        }
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
        
	    } else{
	        argv[i] = NULL;
	}
}

int check_pipe(){
    pipecount = 0;
    for (i=0; i < sizeofargv; i++){
        if (!strcmp(argv[i], "|")) {
            pipecount++;
        }
    }
    if (pipecount != 0){
        return 1;
    } else {
        return 0;
    }
}

void without_pipe() {
    pid = fork();
        if(pid < 0){
            fprintf(stderr, "fork() Failed");
            exit(1);
        } else if (pid == 0) {
            // include file path
            execvp(argv[0], argv);
            if (funcerr != 0) {
                exit(1);
            }
            printf("3230shell: '%s': No such file or directory\n", argv[0]);
            exit(1);
        } else {
            if(NULL == argv[i]) {
                waitpid(pid, NULL, 0);
            }
            // remove zombie processes
            waitpid(-1, NULL, WNOHANG);
        }
}

int div_arg() {
    for (int i=0; i<sizeofargv-1; i++) {
        if(!strcmp(argv[i],"|")){
            if(!strcmp(argv[i+1],"|")){
                printf("3230shell: should not have two consecutive | without in-between command\n");
                return 1;
            }
        }
    }
    // divide into cases depending on the number of pipes (max 4)
    switch (pipecount) {
        case 1:
            printf("");
            int spot;

            for(int i=0; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot = i;
                }
            }

            for(int i=0; i<spot; i++){
                argv1[i] = argv[i];
            }

            int j = 0;
            for(int i=spot+1; i<sizeofargv; i++){
                argv2[j] = argv[i];
                j++;
            }

            break;
        case 2:
            printf("");
            int spot2_1;
            int spot2_2;

            for(int i=0; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot2_1 = i;
                    break;
                }
            }

            for(int i=spot2_1 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot2_2 = i;
                }
            }

            for(int i=0; i<spot2_1; i++){
                argv1[i] = argv[i];
            }

            int k = 0;
            for(int i=spot2_1+1; i<spot2_2; i++){
                argv2[k] = argv[i];
                k++;
            }

            int l = 0;
            for(int i=spot2_2+1; i<sizeofargv; i++){
                argv3[l] = argv[i];
                l++;
            }
            break;
        case 3:
            printf("");
            int spot3_1;
            int spot3_2;
            int spot3_3;

            for(int i=0; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot3_1 = i;
                    break;
                }
            }

            for(int i=spot3_1 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot3_2 = i;
                    break;
                }
            }

            for(int i=spot3_2 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot3_3 = i;
                }
            }

            for(int i=0; i<spot3_1; i++){
                argv1[i] = argv[i];
            }

            int m = 0;
            for(int i=spot3_1+1; i<spot3_2; i++){
                argv2[m] = argv[i];
                m++;
            }

            int n = 0;
            for(int i=spot3_2+1; i<spot3_3; i++){
                argv3[n] = argv[i];
                n++;
            }

            int o = 0;
            for(int i=spot3_3+1; i<sizeofargv; i++){
                argv4[o] = argv[i];
                o++;
            }

            break;
        case 4:
            printf("");

            int spot4_1;
            int spot4_2;
            int spot4_3;
            int spot4_4;

            for(int i=0; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot4_1 = i;
                    break;
                }
            }

            for(int i=spot4_1 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot4_2 = i;
                    break;
                }
            }

            for(int i=spot4_2 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot4_3 = i;
                    break;
                }
            }

            for(int i=spot4_3 + 1; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot4_4 = i;
                    break;
                }
            }            

            for(int i=0; i<spot4_1; i++){
                argv1[i] = argv[i];
            }

            int p = 0;
            for(int i=spot4_1+1; i<spot4_2; i++){
                argv2[p] = argv[i];
                p++;
            }

            int q = 0;
            for(int i=spot4_2+1; i<spot4_3; i++){
                argv3[q] = argv[i];
                q++;
            }

            int r = 0;
            for(int i=spot4_3+1; i<spot4_4; i++){
                argv4[r] = argv[i];
                r++;
            }

            int s = 0;
            for(int i=spot4_4+1; i<sizeofargv; i++){
                argv5[s] = argv[i];
                s++;
            }

            break;
        
            default:
                printf("too many pipes!!!\n");
                break;
        return 1;
    }
}

void close_mfd(int mfd[][2], int i, int total){
    for (int j=0; j<total; j++){
        if (i==j){
            close(mfd[j][1]);
            continue;
        }
        if (i+j == j){
            close(mfd[j][0]);
            continue;
        }
        close(mfd[j][0]);
        close(mfd[j][1]);
    }
}

int with_pipe(){
    if (pipecount == 1) {
        int fd[2];
        if (pipe(fd) == -1) {
            fprintf(stderr, "pipe() Failed");
            exit(-1);
        }

        int pid1 = fork();
        if (pid < 0) {
            fprintf(stderr, "fork() Failed");
            exit(-1);
        } else if (pid1 == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(argv1[0], argv1);
            if (funcerr != 0) {
                exit(1);
            }
            printf("3230shell: '%s': No such file or directory\n", argv1[0]);
            exit(1);
        }

        int pid2 = fork();
        if (pid2 < 0) {
            fprintf(stderr, "pipe() Failed");
            exit(-1);
        } else if (pid2 == 0) {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(argv2[0], argv2);
            if (funcerr != 0) {
                exit(1);
            }
            printf("3230shell: '%s': No such file or directory\n", argv2[0]);
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        return 0;

    } else {

    }
}