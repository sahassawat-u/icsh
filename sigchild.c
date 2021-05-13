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
// typedef struct Job_t Job;
// typedef struct JobList_t JobList;
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
    // int is_bg;
    // int job_id;
};
// struct sigaction action;

// struct JobList {
//    struct Job job_entry;
//    struct JobList *next_job;
// };
// char jobs[MAX][MAX];
int child_control,childID;
struct Job jobs[MAX];
char latest_command[64],global_color[10];
int size_job, is_foreground_process,stop,interrupt, exit_status;
int find_first_job_id_available() {
    for (int i=0;i<MAX;i++) {
        if (jobs[i].status == EMPTY || jobs[i].status == DELETED){
            // printf("status = %d\n",i);
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
    // printf("job id (%d) and size job (%d)",job_idx,size_job);
    if(job_idx==size_job)size_job++;
    return job_idx;
}
int child_info(pid_t pid) {
    int flag;
    for(int i =0;i<size_job;i++) {
        if (jobs[i].pid == pid && jobs[i].status == OCCUPIED) {
            flag = i;
            // break;
        }
    }
    return jobs[flag].child_show_info;
}
void print_job(pid_t pid) {
    int flag;
    // printf("pid (%d)",pid);
    for(int i =0;i<size_job;i++) {
        // printf("i = %d\n",i);
        if (jobs[i].pid == pid && jobs[i].status == OCCUPIED) {
            flag = i;
            // printf("index = %d and pid = %d ",i,pid);
            // jobs[i].status = DELETED;
            break;
        }
    }
    // size_job--;
    if(!child_control){
        printf("\n[%d]+ Done        %s\n",flag+1,jobs[flag].command);
    } else {
        printf("[%d]+ Done        %s\n",flag+1,jobs[flag].command);
    }
    // delete 
    // for(int i=flag)
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
// void deleteJob(pid_t pid) {

// }
void kill_zombies(int sig) {
    int status;
    pid_t pid;
    // printf("hello from zombie");
    // reap exiting child 
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        exit_status = WEXITSTATUS(status);
        if(WIFEXITED(status)){
            // exit_status = WEXITSTATUS(status);
            // printf("exit status (%d)",exit_status);
            if(pid == childID) {
                child_control = 0;
            }
            if (child_info(pid)){
                print_job(pid);
            }
            delete_job(pid);
            //  printf("hello signal\n");
            // (pid);
            // printf("\nReaping zombie pid (%d)\n",pid);
        } 
        if(WIFSIGNALED(status)){
            printf("hello signal\n");
            // exit_status = WEXITSTATUS(status);
        }
        if(WIFSTOPPED(status)){
            printf("hello stop\n");
            // exit_status = WEXITSTATUS(status);
        }
        if(WIFCONTINUED(status)){
            printf("hello cont\n");
            // exit_status = WEXITSTATUS(status);
        }
        // printf("hello\n");
            // printf("\nReaping zombie pid (%d)\n",pid);
    }
    // printf("hello\n");
    // exit_status = WEXITSTATUS(status);
    // printf("exit status (%d)",exit_status);
    
}
// void add_job(struct Job *job) {
    
// }
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
    // printf(" hello");
}
void fg_command(int job_id,int len) {
    // printf("len (%d)\n",len);
    int status;
    // memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
    // int job_id = atoi(argv[1]);
    // --job_id;
    // printf("%s\n",jobs[job_id].command);
    // printf("len (%d)\n",len);
    if (len < 3) {
        // printf("usage: fg <pid>\n");
        return ;
    }
    // printf("pid (%d)\n", getpid());
    // struct sigaction child_action;
    // child_action.sa_handler = kill_zombies;
    // sigemptyset(&child_action.sa_mask);
    // child_action.sa_flags = 0;
    // pid_t pid;
    // printf("hello");
    // int status;
    // memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
    // int job_id = atoi(argv[1]);
    // --job_id;
    // printf("hello");
    // signal(SIGINT, SIG_IGN);
    int child_id = jobs[job_id].pid;
    // printf("child id in fg (%d)\n",child_id);
    jobs[job_id].shstatus = "Running";
    jobs[job_id].child_show_info = 0;
    child_control = 1;
    childID = child_id;
    // printf("%s",jobs[job_id].command);
    // printf("[%d]+ %s        %s&",job_id+1,
    // jobs[job_id].shstatus,jobs[job_id].command);
    // printf("%s",jobs[job_id].command);
    // signal(SIGCHLD, SIG_DFL);
    // int child_id = jobs[job_id].pid;
    // printf("pid in fg (%d)",child_id);
    // int set_fg = tcsetpgrp(0, child_id);
    // signal(SIGTTOU, SIG_IGN);
    // signal(SIGTTIN, SIG_IGN);
    setpgid(child_id, child_id);
    tcsetpgrp(0, child_id);
    // printf("fg successful (%d)\n",tcsetpgrp(0, child_id));
    // printf("process group (%d)\n",job_id);
    // signal(SIGCHLD, SIG_DFL);
    waitpid(child_id, &status, 0);
    // if (WIFEXITED(status)){
    //     // printf("finished!\n");
    //     // delete_job(child_id);
    //     // sigaction(SIGCHLD, &child_action, NULL); 
    //     // signal(SIGTTOU, SIG_IGN);
    //     // signal(SIGTTIN, SIG_IGN);
    //     // tcsetpgrp(0, getpid());
    //     // printf("fg pid (%d)\n",getpid());
    //     // printf("process group (%d)\n",getppid());
    //     // signal(SIGTTOU, SIG_DFL);
    //     // signal(SIGTTIN, SIG_DFL);
    //     // printf("finished!\n");
    //     printf("terminated\n");
    // // printf("finished bg\n");
    // } 
    // else if (WIFSIGNALED(status)) {
    //    printf("signaled\n");
    // } else if (WSTOPSIG(status)) {
    //     // status = -1;
    //     printf("stop\n");
    // }
    // if (status == 1) {
    //     printf("error\n");
    // }
    // while((wait(&status) != job_id)){
    //     // printf("wait (%d)",wait(&status));
    // }
    // printf("fg pid (%d)\n",getpid());
    // signal(SIGCHLD, SIG_DFL);
    // sigaction(SIGCHLD, &child_action, NULL); 
    // signal(SIGTTOU, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, getpid());
    // printf("fg pid (%d)\n",getpid());
    // printf("process group (%d)\n",getppid());
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    // printf("finished bg\n");
    // child_control = 0;
}
void bg_command(char **argv,int len) {
    if (len < 3) {
        return ;
    }
    memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
    int job_id = atoi(argv[1]);
    --job_id;
    printf("[%d]+ %s &\n",job_id+1,jobs[job_id].command);
    kill(jobs[job_id].pid,SIGCONT);
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
        char *stripped_command = jobs[job_id].command + 1;
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
        // printf("argv (%s)\n",argv[i]);
        if(!strcmp(argv[i], "&") && i > 0) {
            is_background_process = 1;
            i++;
            // break;   
        }
        else if(!strcmp(argv[i],"jobs") && size_job>0) {
            // printf("hello ");
            printJobs();
            return;
        }
        else if (!strcmp(argv[i], "<")) {        // looking for input character
            ++i;
            has_input = 1;
            strcpy(input_argv, argv[i]);   
            continue;
        }                                   // end input chech

        else if (!strcmp(argv[i], ">")) {        // looking for output character
            ++i;
            has_output = 1;
            strcpy(output_argv, argv[i]);   
            continue;
        }
        // printf("argv (%s) (%d)\n",argv[i],idx);
        strcat(command_for_bg, " ");
        strcat(command_for_bg, argv[i]);   
        stripped_argv[idx++] = argv[i];
    }
    stripped_argv[idx-1] = (char*)NULL;
    // printf("parent pid (%d)\n",getpid());
    // char *stripped_argv[n];
    // int idx = 0;
    // char command_for_bg[64] = "";
    // for (int j = 0; j < n; j++) {
    //     // printf("%d\n",j);
    //     if(!strcmp(argv[j], "&")) {
    //         ++j;
    //     }
    //     if (!strcmp(argv[j], "<")) {        // looking for input character
    //         ++j;
    //         if ((in = open(argv[j-2], O_RDONLY)) < 0) {   // open file for reading
    //             fprintf(stderr, "error opening file\n");
    //         }
    //         // j+=3;
    //         dup2(in, STDIN_FILENO);         // duplicate stdin to input file
    //         close(in);                      // close after use
    //         continue;
    //     }                                   // end input chech

    //     if (!strcmp(argv[j], ">")) {        // looking for output character
    //         ++j;
    //         // printf("%s\n",argv[j]);
    //         //  printf("hello 0\n");
    //         // printf("hello\n");
    //         // printf("%d\n",j);
    //         // out = creat(argv[j], 0644); // create new output file
    //         out = open(argv[j], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //         // // int output_fds = open("./output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //         // // out =  open(argv[j], O_TRUNC | O_CREAT | O_WRONLY, 0666);
    //         dup2(out, STDOUT_FILENO);       // redirect stdout to file
    //         close(out);                     // close after usere
    //         // printf("dup2 = %d and cloase = %d\n",,);
    //         continue;
    //     }   
    //     strcat(command_for_bg, " ");
    //     strcat(command_for_bg, argv[j]);
    //     stripped_argv[idx++] = argv[j];
    // } 
    // //  printf("hello 0\n");
    // stripped_argv[idx-1] = (char*)NULL;
    // for(int i =0;i<idx;i++) {
    //     printf("(%s)\n",stripped_argv[i]);
    // }
    // char *comm[] = {"sleep","1",NULL};
    struct sigaction child_action;
    child_action.sa_handler = kill_zombies;
    sigemptyset(&child_action.sa_mask);
    child_action.sa_flags = 0;
    //  printf("hello 0\n");
    if(is_background_process) {
        // jobs[job_idx].command = line;
        // struct Job *new_job = (struct Job*) malloc(sizeof(struct Job));
        // line[strlen(line)-1] = '\0';
        // printf("line %s\n",command_for_bg);
        // printf("%s\n",jobs[job_idx].command);
        // new_job->job_id = 1;
        // new_job->command = line;

        // struct sigaction action;
        // action.sa_handler = kill_zombies;
        // sigemptyset(&action.sa_mask);
        // action.sa_flags = 0;
        sigaction(SIGCHLD, &child_action, NULL);
        int job_id = find_first_job_id_available();
        pid_t pid;
        pid = fork();
        if (pid < 0) { 
          printf("forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
            if (has_input) {        // looking for input character
                if ((in = open(input_argv, O_RDONLY)) < 0) {   // open file for reading
                    fprintf(stderr, "ERROR opening file\n");
                    return;
                }
                // j+=3;
                dup2(in, STDIN_FILENO);         // duplicate stdin to input file
                close(in);                      // close after use
            }    
            if (has_output) {        // looking for output character
                out = open(output_argv, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                // free(output_argv);
                dup2(out, STDOUT_FILENO);       // redirect stdout to file
                close(out);                     // close after usere
            }   
            // input_argv = "";
            // printf("children pid (%d)\n",getpid());
            // setpgid(0,getpid());
            // tcsetpgrp (0, getpid());
            // printf("pgid (%d)\n",getppid());
            setpgid(0,getpid());
            // setpgid(0,0);
            // tcsetpgrp(0, getpid());
            // printf("hello 1\n");
            // printf("success (%d)\n",success);
            // printf("pgid (%d)\n",getppid());
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{
            // printf("hello 1\n");
            // setpgid(pid, pid);
            // tcsetpgrp (0, pid);
            add_to_bg_jobs(command_for_bg,pid,"Running");
            printf("[%d] %d\n",job_id+1,pid); 
        }
    } else {
        // sigaction(SIGCHLD, &child_action, NULL); 
        signal(SIGCHLD, SIG_DFL);
        // printf("hello 1\n");
        int status;
        pid_t pid;
        pid = fork();
        if (pid < 0) {    
           printf("forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
           if (has_input) {        
               printf("input file (%s)\n",input_argv);
                if ((in = open(input_argv, O_RDONLY)) < 0) {   // open file for reading
                    fprintf(stderr, "ERROR opening file\n");
                    return;
                }
                // for(int i=0;input_argv[i]!=NULL;i++){
                //     free(input_argv[i]);
                // }
                // free(input_argv);
                // j+=3;
                dup2(in, STDIN_FILENO);         // duplicate stdin to input file
                close(in);                      // close after use
            }    
            if (has_output) {        // looking for output character
                out = open(output_argv, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(out, STDOUT_FILENO);       // redirect stdout to file
                close(out);                     // close after usere
            }   
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{
            // printf("hello 3\n");
            is_foreground_process = 1;
            // printf("interrupt = %d\n",interrupt);
            while ((wait(&status) != pid && !stop)){
                exit_status = WEXITSTATUS(status);
                // printf("hello\n");
                // printf("interrupt = %d\n",interrupt);
            };
            // exit_status = WEXITSTATUS(status);
            //     printf("exit status (%d)",exit_status);
            // printf("finished...\n");
            if(stop){
                // printf("pid = %d\n",pid);
                add_to_bg_jobs(latest_command,pid,"suspended");
            }
            // printf("finished\n");
        }
        // printf("hello\n");
        sigaction(SIGCHLD, &child_action, NULL); 
    }
}
void red(){
    printf(PURPLE);
}
void to_upper(char *color) {
    int n = strlen(color); //<-- getting the length of 's'.  
    char COLOR[n]; 
    for (int i = 0; i < n; i++)
    {
        COLOR[i] = toupper(color[i]); 
    }
    strcpy(color,COLOR);
    return;
} 
void read_line(char *line) {
    char *argv[64];
    split(line, argv);
    // for(int i=0;argv[i]!=NULL;i++) {
    //     printf("[%s] ",argv[i]);
    // }
    if(!strcmp(argv[0],"echo") && !strcmp(argv[1],"$?")) {
        printf("%d\n",exit_status);
    }
    else if(!strcmp(argv[0],"color")) {
        char color[10];
        strcpy(color,argv[1]);
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
        }
        // printf(argv[1]);
    }
    else if (!strcmp(argv[0],"exit")) {
        exit(atoi(argv[1]));
    } 
    else if (!strcmp(argv[0],"!!")) {
        // printf("maintenance...\n");
        // size_t n_latest_command = strlen(latest_command);
        // char tmp;
        // memcpy(tmp,latest_command[strlen(latest_command)-1]);
        // latest_command[strlen(latest_command)-1] = '\0';
        char displayed_command[20];
        strcpy(displayed_command,latest_command);
        displayed_command[strlen(displayed_command)-1] = '\0';
        printf("%s",displayed_command);
        char input[1];
        // scanf("%s",input);
        fgets(input,25,stdin);
        read_line(latest_command);
        // printf("\n");
    }
    else
        execute(argv);
    // 
}
void check_CZ(int sig) {
    // printf("hello from CZ\n");
    // signal(SIGTSTP, SIG_DFL);
    // int status;
    pid_t pid;
    // exit_status = 130;
    if(is_foreground_process) {
        printf("\n[1]+ suspended       %s\n",latest_command);
        kill(pid,SIGTSTP);
        stop = 1;
        exit_status = 130;
        // printf("pid (%d)\n",pid);
        // add_to_bg_jobs(latest_command,pid,"suspended");
        // printf("%s\n",latest_command);
        // int job_idx = find_frst_job_id_available();
        // if(job_idx!=-1) {
        //     strcpy(jobs[job_idx].command,last_command);
        //     jobs[job_idx].status = OCCUPIED;
        //     jobs[job_idx].pid = pid; 
        //     size_job++;
        //     printf("[%d] \n",pid); 
        // }
        // signal(SIGTSTP, SIG_IGN);
        // return;
    }
     else {
        printf("\n");
    }
    // signal(SIGTSTP, SIG_DFL);
    // exit(1);
    // raise (SIGTSTP);
}
void check_CC(int sig) {
    // printf("hello from CZ\n");
    // signal(SIGTSTP, SIG_DFL);
    // int status;
    pid_t pid;
    // exit_status = WEXITSTATUS(status);
    if(is_foreground_process) {
        // kill(pid, SIGKILL);
        // printf("hello from CC\n");
        interrupt = 1;
        printf("\n");
        exit_status = 146;
        // stop = 1;
        // signal(SIGTSTP, SIG_IGN);
        // return;
    } else {
        printf("\n");
    }
    // signal(SIGTSTP, SIG_DFL);
    // exit(1);
    // raise (SIGTSTP);
}
void start_shell() {
    printf(BLUE "Starting IC shell\n");
    strcpy(global_color,PURPLE);
    // global_color = WHITE;
    // jobs = (struct Job*) malloc(sizeof(struct Job));
    // struct sigaction action;
    // action.sa_handler = kill_zombies;
    // sigemptyset(&action.sa_mask);
    // action.sa_flags = 0;
    // action.sa_handler = kill_zombies;
    // sigemptyset(&action.sa_mask);
    // action.sa_flags = 0;
    pid_t pid = getpid();
    // printf("pid (%d)\n",pid);
    // printf("shell pid (%d)\n",pid);
    setpgid(pid, pid);
    tcsetpgrp(0, pid);
    exit_status = 0;
    stop = 0,interrupt = 0,childID = -1;
    struct sigaction CZ_action;
    CZ_action.sa_handler = check_CZ;
    sigemptyset(&CZ_action.sa_mask);
    CZ_action.sa_flags = 0;
    struct sigaction CC_action;
    CC_action.sa_handler = check_CC;
    sigemptyset(&CC_action.sa_mask);
    CC_action.sa_flags = 0;
    // sigaction(SIGTSTP, &CZ_action, NULL); 
    is_foreground_process = 0;
    for (int i =0;i<MAX;i++){
        jobs[i].status = EMPTY;
    }
    child_control = 0;
    while(1){
        // sigaction(SIGCHLD, &child_action, NULL); 
        sigaction(SIGTSTP, &CZ_action, NULL); 
        // sigaction(SIGINT, &CC_action, NULL); 
        // printf("child control (%d)\n",child_control);
        if(!child_control){
            printf(GREEN "icsh $ ");
            printf("%s",global_color);
            char line[64];
            if(fgets(line, sizeof(line), stdin)){
                // char tmp_command[64];
                // strcpy(tmp_command, line);
                // line[strlen(line)-1]='\0';
                if(strcmp("!!\n",line)){
                    strcpy(latest_command, line);
                    // latest_command[strlen(latest_command)-1]='\0';
                }
                read_line(line);
                // printf("%s ",line);
                // strcpy(latest_command, tmp_command);
                // printf("%s",latest_command);
            }
        }
        // else {
        //     pause();
        //     // child_control = 0;
        // }
        // char *line;
        // size_t buffsize = 0;
        // getline(&line, &buffsize, stdin);
        // printf("%s",args);
        // line[strlen(line)-1]='\0';
        // char tmp_command[64];
        // strcpy(tmp_command, argv);
        // strcpy(latest_command, tmp_command);
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
    // printf("\033[1;37;41m" "mysh>");
    if(argc>1){
        run_file(argc,argv);
    }
    else {
        start_shell();
    }
    // struct sigaction action;
    // action.sa_handler = kill_zombies;
    // action.sa_flags = 0;
    // sigaction(SIGCHLD, &action, NULL);
    // signal(SIGCHLD, SIG_DFL);
    // while(1){
    //     char word[64];
    //     printf("test $ ");
    //     // size_t buffsize = 0;
    //     fgets(word, sizeof(word), stdin);
    //     pid_t pid;
    //     // kill_zombies();
    //     // signal(SIGCHLD, SIG_DFL);
    //     // struct sigaction action;
    //     // action.sa_handler = kill_zombies;
    //     // action.sa_flags = 0;
    //     // sigaction(SIGCHLD, &action, NULL);
    //     // getline(&word, &buffsize, stdin);

    //         //  command_line[strlen(command_line)-1]='\0';
    //         //   printf("%s\n",command_line);
    //         // if (!strcmp(command_line,"bg")) {
    //             //  printf("hello\n");
    //         // }
    //         // break;
    //         // printf("%s",word);
    //         word[strlen(word)-1]='\0';
    //         // printf("%s",word);
    //         if (!strcmp(word,"&")) {
    //             // printf("hello\n");
    //         struct sigaction action;
    //         action.sa_handler = kill_zombies;
    //         action.sa_flags = 0;
    //         sigaction(SIGCHLD, &action, NULL);
    //         // pid_t pid;
    //         pid = fork();
    //         if (pid < 0) {   
    //             printf("forking child process failed\n");
    //             exit(1);
    //             }
    //         else if(pid == 0) {
    //             // printf("hello\n");
    //             char *comm[3] = {"sleep","2",NULL};
    //             // printf("hello\n");
    //             if (execvp("sleep", comm)==-1) {  
    //             printf("bad command\n");
    //             exit(1);
    //         }
    //         } else{ 
    //             // don't wait, let it be zombie 
    //             printf("[%d] \n",pid); 
    //         }
    //     } 
    //     // wait
    //     else if(!strcmp(word,"fg")){
    //         // free(x);
    //         signal(SIGCHLD,SIG_DFL);
    //         // pid_t pid;
    //         int status;
    //         pid = fork();
    //         if (pid < 0) {   
    //             printf("forking child process failed\n");
    //             exit(1);
    //             }
    //         else if(pid == 0) {
    //             printf("sleeping...\n");
    //             char *comm[3] = {"sleep","1",NULL};
    //             if (execvp("sleep", comm)==-1) {  
    //             printf("bad command\n");
    //             exit(1);
    //         }
    //         } else{
    //             while (wait(&status) != pid);
    //         }
    //         printf("finished...\n");
    //     }
    // }
    // printf("Done\n");
    return 0;
}

// while(1){
//         char word[64];
//         printf("test $ ");
//         size_t buffsize = 0;
//         fgets(word, sizeof(word), stdin);
//         // getline(&command_line, &buffsize, stdin);

//             //  command_line[strlen(command_line)-1]='\0';
//             //   printf("%s\n",command_line);
//             // if (!strcmp(command_line,"bg")) {
//                 //  printf("hello\n");
//             // }
//             // break;
//             // printf("%s",word);
//             word[strlen(word)-1]='\0';
//             if (!strcmp(word,"bg")) {
//             struct sigaction action;
//             action.sa_handler = kill_zombies;
//             action.sa_flags = 0;
//             sigaction(SIGCHLD, &action, NULL);
//             pid_t pid;
//             pid = fork();
//             if (pid < 0) {   
//                 printf("forking child process failed\n");
//                 exit(1);
//                 }
//             else if(pid == 0) {
//                 char *comm[] = {"sleep","1",NULL};
//                 if (execvp("sleep", comm)==-1) {  
//                 printf("bad command\n");
//                 exit(1);
//             }
//             } else{ 
//                 // don't wait, let it be zombie 
//                 printf("[%d] \n",pid); 
//             }
//         } 
//         // wait
//         else if(!strcmp(word,"fg")){
//             // free(x);
//             signal(SIGCHLD,SIG_DFL);
//             pid_t pid;
//             int status;
//             pid = fork();
//             if (pid < 0) {   
//                 printf("forking child process failed\n");
//                 exit(1);
//                 }
//             else if(pid == 0) {
//                 printf("sleeping...\n");
//                 sleep(5);
//                 exit(0);
//             } else{
//                 while (wait(&status) != pid);
//             }
//             printf("finished...\n");
//         }
//     }
//     printf("Done\n");




// int n = 0, in, out;
//     while(argv[n]!=NULL)n++;
//     char *stripped_argv[n];
//         int idx = 0;
//         for (int j = 0; j < n; j++) {
//             if(!strcmp(argv[j], "&")) {
//                 ++j;
//                 // is_waiting = 1;   
//             }
            // printf("%s %d\n",argv[j],j);
            // if (!strcmp(argv[j], "<")) {        // looking for input character
            //     ++j;
            //     if ((in = open(argv[j-1], O_RDONLY)) < 0) {   // open file for reading
            //         fprintf(stderr, "error opening file\n");
            //     }
            //     // j+=3;
            //     dup2(in, STDIN_FILENO);         // duplicate stdin to input file
            //     close(in);                      // close after use
            //     continue;
            // }                                   // end input chech

            // if (!strcmp(argv[j], ">")) {        // looking for output character
            //     ++j;
            //     // printf("hello\n");
            //     // printf("%d\n",j);
            //     // out = creat(argv[j], 0644); // create new output file
            //     out = open(argv[j++], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            //     // // int output_fds = open("./output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            //     // // out =  open(argv[j], O_TRUNC | O_CREAT | O_WRONLY, 0666);
            //     dup2(out, STDOUT_FILENO);       // redirect stdout to file
            //     close(out);                     // close after usere
            //     // printf("dup2 = %d and cloase = %d\n",,);
            //     continue;
            // }                                   // end output check
            // if (!strcmp(argv[j],"&")) {
            //     ++j;
            //     jobId++;
            //     printf("[%d] %d",jobId,pid);
            // }
            // if (!strcmp(argv[j], ">>")) {       // looking for append
            //     ++j;
            //     int append = open(argv[j], O_CREAT | O_RDWR | O_APPEND, 0644);

            //     dup2(append, STDOUT_FILENO);
            //     close(append);
            //     continue;
            // }

            // stripped_argv[idx++] = argv[j];
        // } 
        // printf("%d\n", not_waiting);
        // stripped_argv[idx-1] = (char*)NULL;