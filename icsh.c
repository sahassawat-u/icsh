#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <stdio.h>
#include <ctype.h>
#define MAX 64
#define OCCUPIED 2
#define DELETED 1
#define EMPTY 0
#define MAX_LEN 256
#define NONE "\033[m"
#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32;32m"
#define GRAY "\033[1;30m"
#define WHITE "\033[0;37m"
#define PURPLE "\033[0;35m"
#define BLUE "\033[0;34m"
struct Job {
    char command[64];
    pid_t pid;
    int status;
    char *shstatus;
    int child_show_info;
};

int child_control,child_id;
struct Job jobs[MAX];
char latest_command[64],global_color[10];
int size_job, is_foreground_process,stop,interrupt, exit_status;
int find_first_job_id_available() {
    for (int i=0;i<MAX;i++) {
        if (jobs[i].status == EMPTY || jobs[i].status == DELETED){
            return i;
        }
    }
    return -1;
}
int add_to_bg_jobs(char *command,pid_t pid, char *shstatus) {
    int job_idx = find_first_job_id_available();
    if(job_idx!=-1) {
        strcpy(jobs[job_idx].command,command);
        jobs[job_idx].status = OCCUPIED;
        jobs[job_idx].pid = pid; 
        jobs[job_idx].shstatus = shstatus; 
        jobs[job_idx].child_show_info = 1;
    }
    if(job_idx==size_job)size_job++;
    return job_idx;
}
int child_info(pid_t pid) {
    int flag;
    for(int i =0;i<size_job;i++) {
        if (jobs[i].pid == pid && jobs[i].status == OCCUPIED) {
            flag = i;
            break;
        }
    }
    return jobs[flag].child_show_info;
}
void print_job(pid_t pid) {
    int flag;
    for(int i =0;i<size_job;i++) {
        if (jobs[i].pid == pid && jobs[i].status == OCCUPIED) {
            flag = i;
            break;
        }
    }
    if(!child_control){
        printf("\n[%d]+ Done        %s\n",flag+1,jobs[flag].command);
    } else {
        printf("[%d]+ Done        %s\n",flag+1,jobs[flag].command);
    }
}
void delete_job(pid_t pid) {
    for(int i =0;i<size_job;i++) {
        if (jobs[i].pid == pid && jobs[i].status == OCCUPIED) {
            jobs[i].status = DELETED;
            kill(pid, SIGSTOP);
            break;
        }
    }
}
void kill_zombies(int sig) {
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        exit_status = WEXITSTATUS(status);
        if(WIFEXITED(status)){
            if(pid == child_id) {
                child_control = 0;
            }
            if (child_info(pid)){
                print_job(pid);
            }
            delete_job(pid);
        } 
        if(WIFSIGNALED(status)){
            // printf("hello signal\n");
        }
        if(WIFSTOPPED(status)){
            // printf("hello stop\n");
        }
        if(WIFCONTINUED(status)){
            // printf("hello cont\n");
        }
    }
}

void split(char *line, char **argv)
{
     while (*line != '\0') {     
        while(*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';   
        *argv++ = line; 
        while (*line != '\0' && *line != ' ' && 
                *line != '\t' && *line != '\n') 
            line++;            
     }
     *argv = NULL;
}
void printJobs() {
    for(int i =0;i<size_job;i++){
        if (jobs[i].status == OCCUPIED){
            printf("[%d]+ %s        %s\n",i+1,jobs[i].shstatus,jobs[i].command);
        }
    }
}
int check_job(int job_id) {
    if(jobs[job_id].status==OCCUPIED) {
        return 1;
    }
    return 0;
}
void fg_command(int job_id,int len) {
    int status;
    if (len < 3) {
        return ;
    }
    if(check_job(job_id)){
    int tmp_child_id = jobs[job_id].pid;
    int is_suspended = 0;
    if(!strcmp(jobs[job_id].shstatus,"suspended")) {
        is_suspended = 1;
    } else {
        jobs[job_id].shstatus = "Running";
        jobs[job_id].child_show_info = 0;
        child_control = 1;
        child_id = tmp_child_id;
    }
    setpgid(child_id, child_id);
    tcsetpgrp(0, child_id);
    if(is_suspended){
        kill(jobs[job_id].pid,SIGCONT);
    }
    waitpid(child_id, &status, 0);
    exit_status = WEXITSTATUS(status);
    // if (WIFEXITED(status)){
    // } 
    // else if (WIFSIGNALED(status)) {
    // } else if (WSTOPSIG(status)) {
    // }
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, getpid());
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    } else {
        printf("fg: (%d): no such job\n",job_id);
    }
    // child_control = 0;
}
void bg_command(char **argv,int len) {
    if (len < 3) {
        return ;
    }
    memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
    int job_id = atoi(argv[1]);
    --job_id;
    if(check_job(job_id)){
        printf("[%d]+ %s &\n",job_id+1,jobs[job_id].command);
        kill(jobs[job_id].pid,SIGCONT);
    } else {
        printf("bg: (%d): no such job\n",job_id);
    }
}
void  execute(char **argv)
{
    int is_background_process = 0;
    int n = 0, in, out;
    while(argv[n]!=NULL)n++;
    if(!strcmp(argv[0],"fg")){
        memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
        int job_id = atoi(argv[1]);
        --job_id;
        char stripped_command[20];
        if(jobs[job_id].command[0] != ' ') {
            strcpy(stripped_command,jobs[job_id].command);
            stripped_command[strlen(stripped_command)]='\0';
        } else {
            strcpy(stripped_command,jobs[job_id].command + 1);
        }
        printf("%s\n",stripped_command);
        fg_command(job_id,n);
        return;
    }
    if(!strcmp(argv[0],"bg")){
        bg_command(argv,n);
        return;
    }
    char command_for_bg[64] = "", output_argv[64],input_argv[64];
    strcmp(input_argv,"");
    strcmp(output_argv,"");
    char *stripped_argv[n];
    int idx = 0, has_output = 0, has_input = 0;
    for(int i =0;i<n;i++){
        if(!strcmp(argv[i], "&") && i > 0) {
            is_background_process = 1;
            i++;
        }
        else if(!strcmp(argv[i],"jobs") && size_job>0) {
            printJobs();
            return;
        }
        else if (!strcmp(argv[i], "<")) {
            ++i;
            has_input = 1;
            strcpy(input_argv, argv[i]);   
            continue;
        }                                  
        else if (!strcmp(argv[i], ">")) {       
            ++i;
            has_output = 1;
            strcpy(output_argv, argv[i]);   
            continue;
        }
        strcat(command_for_bg, " ");
        strcat(command_for_bg, argv[i]);   
        stripped_argv[idx++] = argv[i];
    }
    stripped_argv[idx-1] = (char*)NULL;
    struct sigaction child_action;
    child_action.sa_handler = kill_zombies;
    sigemptyset(&child_action.sa_mask);
    child_action.sa_flags = 0;
    if(is_background_process) {
        sigaction(SIGCHLD, &child_action, NULL);
        int job_id = find_first_job_id_available();
        pid_t pid;
        pid = fork();
        if (pid < 0) { 
          printf("forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
            if (has_input) {
                if ((in = open(input_argv, O_RDONLY)) < 0) {
                    fprintf(stderr, "ERROR opening file\n");
                    return;
                }
                dup2(in, STDIN_FILENO);
                close(in);
            }    
            if (has_output) {
                out = open(output_argv, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(out, STDOUT_FILENO);
                close(out);
            }   
            setpgid(0,getpid());
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{
            add_to_bg_jobs(command_for_bg,pid,"Running");
            printf("[%d] %d\n",job_id+1,pid); 
        }
    } else {
        signal(SIGCHLD, SIG_DFL);
        int status;
        pid_t pid;
        pid = fork();
        if (pid < 0) {    
           printf("forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
           if (has_input) {        
                if ((in = open(input_argv, O_RDONLY)) < 0) {
                    fprintf(stderr, "ERROR opening file\n");
                    return;
                }
                dup2(in, STDIN_FILENO);         
                close(in);                     
            }    
            if (has_output) {       
                out = open(output_argv, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(out, STDOUT_FILENO);
                close(out);                     
            }   
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{
            is_foreground_process = 1;
            while ((wait(&status) != pid && !stop) && !interrupt){
                // printf()
                 exit_status = WEXITSTATUS(status);
            }
            if(interrupt){
                interrupt = 0;
            }
            // exit_status = WEXITSTATUS(status);
            if(stop){
                latest_command[strlen(latest_command)-1]='\0';
                add_to_bg_jobs(latest_command,pid,"suspended");
                stop = 0;
            }
            is_foreground_process = 0;
        }
        sigaction(SIGCHLD, &child_action, NULL); 
    }
}

void to_upper(char *color) {
    int n = strlen(color);
    char COLOR[n]; 
    for (int i = 0; i < n; i++)
    {
        COLOR[i] = toupper(color[i]); 
    }
    strcpy(color,COLOR);
    return;
} 
void set_color(char *fromcolor) {
    char color[10];
    strcpy(color,fromcolor);
    to_upper(color);
    if(!strcmp(color, "RED")){
        strcpy(global_color,RED);
    }
    else if(!strcmp(color, "YELLOW")){
        strcpy(global_color,YELLOW);
    }
    else if(!strcmp(color, "GREEN")){
        strcpy(global_color,GREEN);
    }
    else if(!strcmp(color, "NONE")){
        strcpy(global_color,NONE);
    }
    else if(!strcmp(color, "GRAY")){
        strcpy(global_color,GRAY);
    }
    else if(!strcmp(color, "WHITE")){
        strcpy(global_color,WHITE);
    }
    else if(!strcmp(color, "PURPLE")){
        strcpy(global_color,PURPLE);
    }
    else if(!strcmp(color, "BLUE")){
        strcpy(global_color,BLUE);
    } else {
        printf("No color matched!\n");
    }
}
void print_help() {
    printf("The colors available:\n");
    printf("RED\n");
    printf("YELLOW\n");
    printf("GREEN\n");
    printf("GRAY\n");
    printf("WHITE\n");
    printf("PURPLE\n");
    printf("BLUE\n");
}
void read_line(char *line) {
    char *argv[64];
    split(line, argv);
    if(!strcmp(argv[0],"echo") && !strcmp(argv[1],"$?")) {
        printf("%d\n",exit_status);
    }
    else if(!strcmp(argv[0],"color")) {
        if(!strcmp(argv[1],"--help"))
            print_help();
        else
            set_color(argv[1]);
    }
    else if (!strcmp(argv[0],"exit")) {
        exit(atoi(argv[1]));
    } 
    else if (!strcmp(argv[0],"!!")) {
        char displayed_command[20];
        strcpy(displayed_command,latest_command);
        displayed_command[strlen(displayed_command)-1] = '\0';
        printf("%s",displayed_command);
        char input[1];
        fgets(input,25,stdin);
        read_line(latest_command);
    }
    else
        execute(argv);
}
void check_CZ(int sig) {
    pid_t pid;
    exit_status = 146;
    // printf("pid (%d)\n",pid);
    if(is_foreground_process) {
        int idx = find_first_job_id_available();
        // char copied_command[20];/
        // strcpy(copied_command,latest_command);
        // copied_command[strlen(copied_command)-1]='\0';
        if (latest_command[strlen(latest_command)-1]=='\0'){
            printf("\n");
        }
        // printf("c (->%c<-)",latest_command[strlen(latest_command)-1]);
        printf("\n[%d]+ suspended       %s",idx+1,latest_command);
        printf("");
        kill(pid,SIGTSTP);
        stop = 1;
        is_foreground_process = 0;
    }
     else {
        printf("\n");
    }
}
void check_CC(int sig) {
    pid_t pid;
    exit_status = 130;
    if(is_foreground_process && pid) {
        interrupt = 1;
        printf("\n");
        is_foreground_process = 0;
        kill(pid, SIGKILL);
    } else {
        printf("\n");
    }
}
void features_detail(){
    printf("new feature(s) added\n");
    printf("    (1) color command: to change the color of command line\n");
    printf("                     : --help to show info\n");
}
void start_shell() {
    printf("Starting IC shell\n");
    strcpy(global_color,WHITE);
    features_detail();
    pid_t pid = getpid();
    setpgid(pid, pid);
    tcsetpgrp(0, pid);
    exit_status = 0;
    stop = 0,interrupt = 0,child_id = -1;
    struct sigaction CZ_action;
    CZ_action.sa_handler = check_CZ;
    sigemptyset(&CZ_action.sa_mask);
    CZ_action.sa_flags = 0;
    struct sigaction CC_action;
    CC_action.sa_handler = check_CC;
    sigemptyset(&CC_action.sa_mask);
    CC_action.sa_flags = 0;
    is_foreground_process = 0;
    for (int i =0;i<MAX;i++){
        jobs[i].status = EMPTY;
    }
    child_control = 0;
    while(1){
        sigaction(SIGTSTP, &CZ_action, NULL); 
        sigaction(SIGINT, &CC_action, NULL); 
        if(!child_control){
            printf(GREEN "icsh $ ");
            printf("%s",global_color);
            char line[64];
            if(fgets(line, sizeof(line), stdin)){
                if(strcmp("!!\n",line)){
                    strcpy(latest_command, line);
                }
                read_line(line);
            }
        }
    };
}
void run_file(int argc,char **argv){
    for (int i = 1; i < argc; ++i) {
        FILE* fp;
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("Failed: ");
            return;
        }
        char line[MAX_LEN];
        while (fgets(line, MAX_LEN - 1, fp) != NULL)  {
            if(strcmp("!!\n",line)){
                strcpy(latest_command, line);
            }
            read_line(line);
        }
    }
}
int main(int argc, char **argv) {
    if(argc>1){
        run_file(argc,argv);
    }
    else {
        start_shell();
    }
    return 0;
}
