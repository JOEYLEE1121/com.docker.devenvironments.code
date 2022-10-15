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

char *argv1[MAX_SIZE_ARG];
char *argv2[MAX_SIZE_ARG];
char *argv3[MAX_SIZE_ARG];
char *argv4[MAX_SIZE_ARG];
char *argv5[MAX_SIZE_ARG];


pid_t pid;

int i;
int sizeofargv;
int pipecount;

void block_sig();
void get_cmd();
void convert_cmd();
int timeXfunc();
int exitfunc();
int check_pipe();
void without_pipe();
int with_pipe();

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

        if(!strcmp(cmd, "timeX")) {
            timeXfunc();
        }

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

int timeXfunc(){
    if(!strcmp(argv[-1],"&")) {
        printf("3230shell: \"timeX\" cannot be run in background mode");
        return 0;
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
            printf("3230shell: '%s': No such file or directory\n", argv[0]);
            exit(1);
        } else {
            if(NULL == argv[i]) {
                waitpid(pid, NULL, 0);
            }
        }
}

int with_pipe() {
    for (int i=0; i<sizeofargv-1; i++) {
        if(!strcmp(argv[i],"|")){
            if(!strcmp(argv[i+1],"|")){
                printf("3230shell: should not have two consecutive | without in-between command\n");
                return 1;
            }
        }
    }

    switch (pipecount) {
        case 1:

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
    }
}