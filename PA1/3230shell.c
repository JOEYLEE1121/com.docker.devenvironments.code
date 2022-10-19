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


char *argv[MAX_SIZE_ARG]; // user input will be stored here

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
static int READY = 0;
int timeXcmd = 0;

void block_sig();
void get_cmd();
void convert_cmd();
int timeXfunc();
int exitfunc();
void on_sigusr1();
int check_pipe();
void without_pipe();
int div_arg();
int with_pipe();
void handle_sigint(int sig);

void handle_sigusr1(int sig) {
    READY = 1;
}

void remove_element(char *array[], int index, int array_length) {
   for(int i = index; i < array_length - 1; i++) {
    array[i] = array[i + 1];
   }

    for (int i=0; i < sizeofargv; i++) {
        printf("argv[i]: %s\n",argv[i]);
    }
}

char** getArgvByNum(int id) {
    // printf("I'm in\n");
    if (id == 1) return argv1;
    else if (id == 2) return argv2;
    else if (id == 3) return argv3;
    else if (id == 4) return argv4;
    else return argv5;
}

char* returnTimeXcmd() {
    switch (timeXcmd) {
        case 1:
            return argv1[0];
        case 2:
            return argv2[0];
        case 3:
            return argv3[0];
        case 4:
            return argv4[0];
        case 5:
            return argv5[0];
    }
}

int main(){
    funcerr = 0;
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGINT, handle_sigint);
    while(1){
        // gets user input
        get_cmd();

        if(!strcmp(cmd, "")) {
            continue;
        }


        if(cmd[0] == '|' || cmd[strlen(cmd) - 1] == '|') {
            printf("3230shell: invalid |\n");
            continue;
        }
        // use strtok to store variable
        convert_cmd();

        int consecutive_pipes_error_flag = 0;
        // error if "||" exits in input
        for (int i=0; i<sizeofargv; i++) {
            if(!strcmp(argv[i],"||")){
                printf("3230shell: should not have two consecutive | without in-between command\n");
                consecutive_pipes_error_flag = 1;
                break;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
            }
        }
        if (consecutive_pipes_error_flag == 1){
            continue;
        }

        //exit function handling
        if(!strcmp(cmd, "exit")) {
            if (exitfunc() == 1) {
                break;
            }
            else continue;
        }

        if (check_pipe() == 0) {
            if(!strcmp(cmd, "timeX")){
                if (timeXfunc() == 1) {
                    break;
                } 
                else continue;
        }
            without_pipe();
        } else {
            div_arg();
            if(!strcmp(cmd, "timeX")){
                if (timeXfunc() == 1) {
                    break;
                } 
                else continue;
            }
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

void handle_sigint(int sig) {
    printf("\n$$ 3230shell ## ");
    fflush(stdout); 
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
    //error handle for timeX function
    if (!strcmp(argv[sizeofargv-1],"&")){
        printf("3230shell: \"timeX\" cannot be run in background mode\n");
        funcerr++;
        return 0;
    } else if (sizeofargv == 1 && !strcmp(argv[sizeofargv-1],"timeX")){
        printf("\"timeX\" cannot be a standalone command\n");
        funcerr++;
        return 0;
    } else if (pipecount == 0) { // timeX without pipe
        int t_pid = fork();
        if (t_pid < 0) {
            fprintf(stderr, "timeX failed\n");
            exit(-1);
        } else if (t_pid == 0){
            while (READY == 0) {
                usleep(100);
                if (READY == 1) {
                    break;
                }
            }

            for (int i=0; i<sizeofargv; i++){
                timeXtmp[i] = argv[i+1];
                if (i+1 == sizeofargv) {
                    break;
                }
            }
            execvp(timeXtmp[0], timeXtmp);
            exit(0);
        } else {
            kill(pid, SIGUSR1);
            int status;
            struct rusage rusage;
            int ret = wait4(t_pid, &status, 0, &rusage);
            printf("(PID)%d (CMD)%s (user)%.3f s (sys)%.3f s \n",t_pid, argv[1], rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / 1000000.0, rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / 1000000.0);
        }
    } else if (pipecount > 0) {

        int fd[2], in = 0, out;
        
        int total_num_cmds = pipecount + 1;

        int lastpid;

        char *timeXoutput[MAX_SIZE_ARG];

        for (int k = 0; k < total_num_cmds; k++) {
            
            pipe(fd);
            out = fd[1];
            pid_t pid = fork();
            int lastflag = 0;
            
            ++timeXcmd;
            if (pid == 0) {
                
                while (READY == 0) {
                    usleep(10);
                    if (READY == 1) break;
                }
                if (k == total_num_cmds - 1) {
                    dup2(1, 1);
                }
                if (k != total_num_cmds - 1) {
                    dup2(out, 1); 
                }
                dup2(in, 0);

                close(fd[1]);
                close(fd[0]);

                lastflag++;
                printf("parent PID is: %d\n",getppid());
                printf("this is child (PID)%d\n",getpid());
                lastpid = pid;
                execvp(getArgvByNum(k + 1)[0], getArgvByNum(k + 1));
            } else {
                
                kill(pid, SIGUSR1);

                printf("this is parent (PID) %d\n", getpid());
                int status;
                // struct rusage usage;
                // int child_pid = wait4(pid, &status, 0, &usage);

                // if (WIFEXITED(status)) {
                //     int sCode = WEXITSTATUS(status);
                    
                // } else if (WIFSIGNALED(status)) {
                //     int signalNum = WTERMSIG(status);
                // }

                struct rusage rusage;
                int ret = wait4(lastpid, &status, 0, &rusage);
                printf("lastpid (PID) %d\n", ret);
                printf("(PID)%d (CMD)%s (user)%.3f s (sys)%.3f s \n",pid, returnTimeXcmd(), rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / 1000000.0, rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / 1000000.0);
            }
            in = fd[0];
            close(fd[1]);
        }
        close(fd[1]);
        close(fd[0]);

        }
        timeXcmd = 0;
}

void get_cmd(){
    printf("$$ 3230shell ## ");
    // block_sig();
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
            while (READY == 0) {
                usleep(100);
                if (READY == 1) {
                    break;
                }
            }

            execvp(argv[0], argv);
            if (funcerr != 0) {
                exit(1);
            }
            // error handling if the execvp fails.
            char one = '.';
            char two = '/';
            int ascii1 = one;
            int asciia0 = argv[0][0];
            int ascii2 = two;
            int asciia1 = argv[0][1];
            if(ascii1 == asciia0 && ascii2 == asciia1){
                printf("3230shell: '%s': Permission denied\n", argv[0]);
            }
            else{
                printf("3230shell: '%s': No such file or directory\n", argv[0]);
            }
            exit(1);
        } else {
            kill(pid, SIGUSR1);
            if(NULL == argv[i]) {
                waitpid(pid, NULL, 0);
            }
            // remove zombie processes
            waitpid(-1, NULL, WNOHANG);
        }
}

int div_arg() {
    //eror handling
    for (int i=0; i<sizeofargv-1; i++) {
        if(!strcmp(argv[i],"|")){
            if(!strcmp(argv[i+1],"|")){
                printf("3230shell: should not have two consecutive | without in-between command\n");
                return 1;
            }
        }
    }
    // divide into cases depending on the number of pipes (max 4)

    if (!strcmp(cmd,"timeX")){

        for(int i=0; i<sizeofargv-1; i++) {
            argv[i] = argv[i+1];
        }
        argv[sizeofargv-1] = NULL;

        sizeofargv--;
    }

    switch (pipecount) {

        case 1:
            printf("");
            int spot;

            for(int i=0; i<sizeofargv; i++){
                if(!strcmp(argv[i],"|")){
                    spot = i;
                }
            }
            int argv1_len = spot;
            for(int i=0; i<spot; i++){
                argv1[i] = argv[i];
            }
            argv1[argv1_len] = '\0';

            int j = 0;
            for(int i=spot+1; i<sizeofargv; i++){
                argv2[j] = argv[i];
                j++;
            }

            argv2[sizeofargv - spot - 1] = '\0';

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
            argv1[spot2_1] = '\0';

            int k = 0;
            for(int i=spot2_1+1; i<spot2_2; i++){
                argv2[k] = argv[i];
                k++;
            }
            argv2[spot2_2 - spot2_1 - 1] = '\0';

            int l = 0;
            for(int i=spot2_2+1; i<sizeofargv; i++){
                argv3[l] = argv[i];
                l++;
            }

            argv3[sizeofargv - spot2_2 - 1] = '\0';
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

int with_pipe(){
    int fd[2], in = 0, out;
    
    int total_num_cmds = pipecount + 1;

    for (int k = 0; k < total_num_cmds; k++) {
        pipe(fd);
        out = fd[1];
        pid_t pid = fork();
        if (pid == 0) {
            
            while (READY == 0) {
                usleep(10);
                if (READY == 1) break;
            }
            if (k == total_num_cmds - 1) {
                dup2(1, 1);
            }
            if (k != total_num_cmds - 1) {
                dup2(out, 1); 
            }
            dup2(in, 0);

            close(fd[1]);
            close(fd[0]);
            
            execvp(getArgvByNum(k + 1)[0], getArgvByNum(k + 1));
        } else {
            kill(pid, SIGUSR1);
            int status;
            struct rusage usage;
            int child_pid = wait4(pid, &status, 0, &usage);

            if (WIFEXITED(status)) {
                int sCode = WEXITSTATUS(status);
                
            } else if (WIFSIGNALED(status)) {
                int signalNum = WTERMSIG(status);
            }
        }

        in = fd[0];
        close(fd[1]);
    }
    close(fd[1]);
    close(fd[0]);
}