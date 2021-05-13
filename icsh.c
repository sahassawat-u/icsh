#include <stdio.h>
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
// #include "hello.c"

#define MAX_LEN 256
// #define SIGHUP  1   /* Hangup the process */ 
// #define SIGINT  2   /* Interrupt the process */ 
// #define SIGQUIT 3   /* Quit the process */ 
// #define SIGILL  4   /* Illegal instruction. */ 
// #define SIGTRAP 5   /* Trace trap. */ 
// #define SIGABRT 6   /* Abort. */
int jobId;
char last_command[64];
// void try(command, argvs) {
//     // char* command = "ls";
//     // char* argument_list[] = {"ls", "-l", NULL};
 
//     // printf("Before calling execvp()\n");
 
//     // Calling the execvp() system call
//     int status_code = execvp(command, argvs);
 
//     if (status_code == -1) {
//         printf("Process did not terminate correctly\n");
//         exit(1);
//     }
 
//     printf("This line will not be printed if execvp() runs correctly\n");
 
//     // return 0;
// }
void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = NULL;                 /* mark the end of argument list  */
}
void child_handler() {
    int child_status;
    pid_t pid;
    // printf("hello\n");
    // printf("%d\n",sig);
    // // char *x[]= {"sleep","5","&","asd"};
    // // memcpy(local_last_command, &x[0], 3*sizeof(*x));
    // // for (int i =0; i<4;i++) {
    // //     printf("%s\n",local_last_command[i]);
    // // }
    // // if(is_waiting){
    // pid = waitpid(-1, &child_status, WNOHANG);
    while ((pid = waitpid(-1, &child_status, WNOHANG)) > 0){
        // pid = waitpid(-1, &child_status, WNOHANG);
        // waitpid(-1, &child_status, WNOHANG);
        if (WIFEXITED(child_status)){
            // printf("\n");
            printf("\n[%d]+  Done         %d\n",jobId,pid); 
        }
        // break;
        // else if (pid == 0)
        //     break;
        // else if (pid == -1)
        //     break;
        // else{
        //     printf ("Return code: \n");
        //     return;
        // }
            // printf ("The child is gone\n"); /* dead */
        // if (WIFEXITED(child_status)|| WTERMSIG(child_status))
        //  printf("[%d]+  Done    %s\n",jobId,"hi"); 
        // printf("%d\n",pid);
        //  printf("[%d]+  Done    %s\n",jobId,"hi"); 
        // printf ("The child is gone\n"); /* dead */
        // printf("Received signal %d from process %d\n",sig,pid);
    }
    // printf("hello\n");
    // return;
    // exit(0);
    // printf("out\n");
    // return;
    // return;
    // printf("[%d]+  Done    %s\n",jobId,"hi"); 
    // printf("Received signal %d from process %d\n",sig,pid);
    // char local_last_command[100];
    //             int count=0;
    //             while(count < strlen(last_command)-2){
    //                 local_last_command[count] = last_command[count];
    //                 count++;
    //             }
    //             local_last_command[count] = '\0';
    // printf("\n");
    // printf("[%d]+  Done    %s\n",jobId,"hi"); 
}
void  execute(char **argv)
{
    pid_t  pid;
    int status, is_waiting = 0;
    // struct sigaction action;
    // action.sa_handler = child_handler;
    // sigemptyset(&action.sa_mask);
    // action.sa_flags = 0;
    // sigaction(SIGCHLD, &action, NULL);
    // signal(SIGCHLD, SIG_DFL);
    for(int i =0;argv[i]!=NULL;i++){
        if(!strcmp(argv[i], "&")) {
            is_waiting = 1;   
        }
    }
    int n = 0, in, out;
    while(argv[n]!=NULL)n++;
    char *stripped_argv[n];
        int idx = 0;
        for (int j = 0; j < n; j++) {
            if(!strcmp(argv[j], "&")) {
                ++j;
                // is_waiting = 1;   
            }
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

            stripped_argv[idx++] = argv[j];
        } 
        // printf("%d\n", not_waiting);
        stripped_argv[idx-1] = (char*)NULL;
    // int n = 0, in, out;
    // while(argv[n]!=NULL)n++;
    // char *stripped_argv[n];
        // int idx = 0;
        // for (int j = 0; j < n; j++) {
        //     if(!strcmp(argv[j], "&")) {
        //         ++j;
        //         // is_waiting = 1;   
        //     }
        //     stripped_argv[idx++] = argv[j];
        // } 
        // stripped_argv[idx-1] = (char*)NULL;
    // if (is_waiting) {
    //     int background_process_id;
    //     pid_t fork_return;
    //     fork_return = fork();

    //     if (fork_return == 0) {
    //         // printf("[%d] %d\n",++jobId,fork_return); 
    //         background_process_id = getpid();
    //         // addJobToTable(strippedCommand, background_process_id);
    //         setpgid(0, 0);
    //         execvp(stripped_argv[0], stripped_argv);
    //         // execve(executableCommands[0], executableCommands, NULL);
    //         exit(0);
    //     } else {
    //         return;
    //     }
    // }
    // printf("waiting status %d\n",is_waiting);
    if(is_waiting) {
        struct sigaction action;
        action.sa_handler = child_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGCHLD, &action, NULL);
        signal(SIGCHLD, SIG_DFL);
        pid_t pid = fork();
        // sigaction(SIGCHLD, &action, NULL);
        // kill(cid, 31);
        // setpgid(0,0);
        // sigaction(SIGCHLD, &action, NULL);
        // int bg_id;
        if (pid < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
            // sigaction(SIGCHLD, &action, NULL);
            // setpgid(0,0);
            // signal(SIGINT, SIG_DFL);
            // signal(SIGQUIT, SIG_DFL);
            // signal(SIGTSTP, SIG_DFL);
            // signal(SIGTTIN, SIG_DFL);
            // signal(SIGTTOU, SIG_DFL);
            // signal(SIGCHLD, SIG_DFL);
            // bg_id = getpid();
            // printf("\nparent id  = %d\n",getppid());
            // sigaction(SIGCHLD, &action, NULL);
            // setpgid(0,0);
            // setsid();
            // printf("child id = %d\n",getppid());
            // setsid();
            // sigaction(SIGCHLD, &action, NULL);
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{ 
                printf("[%d] %d\n",++jobId,pid); 
                // zombie_kill();
                // return;
            }
    } else {
        // printf("hello its me\n");
        signal(SIGCHLD, SIG_DFL);
        pid_t pid = fork();
        // int bg_id;
        if (pid < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
        }
        else if(pid == 0) {
            // bg_id = getpid();
            if (execvp(stripped_argv[0], stripped_argv)==-1) {  
                printf("bad command\n");
                exit(1);
            }
        } else{
            // tcsetpgrp(0, getppid());
            while (wait(&status) != pid);
            // printf("finished %d\n",getpid());
            // return ;
        } 
    }
    // pid = fork();
    //  if (pid < 0) {     /* fork a child process           */
    //       printf("*** ERROR: forking child process failed\n");
    //       exit(1);
    //  }
    //  else if (pid == 0) {          
    //      // Child process
    //     // sigaction(SIGCHLD, &action, NULL);
    //     if(is_waiting){
    //         printf("hellooo");
    //         setpgid(0,0);
    //     }
    //     // for (int i=0;i<n;i++) {
    //     //     if(!strcmp(argv[i], "<")) {
    //     //         if((in = open(argv[i+1], O_RDONLY)) < 0) {
    //     //             fprintf(stderr, "error opening file\n");
    //     //         }
    //     //         dup2(in, STDIN_FILENO);
    //     //         close(in);
    //     //     }
    //     //     if (!strcmp(argv[i], ">")) {
    //     //         printf("i = %d",i);
    //     //         out = creat(argv[i+1], 0644);
    //     //         dup2(out, STDOUT_FILENO);
    //     //         close(out);
    //     //     }
        
    //     // }
    //     // printf("%d\n",n);
        
    //     //  if (stripped_argv[idx - 1] == 0)
    //         // stripped_argv[idx - 1] = NULL;
    //     // char* cleaned_argv[idx-1];
    //     // for (int i =0; i<idx;i++) {
    //     //     printf("%s\n",stripped_argv[i]);
    //     // }
    //     // cleaned_argv[]
    //     // printf("%d\n", idx);
    //     // // stripped_argv[idx]= NULL;
    //     // for (int i =0; cleaned_argv[i]!=NULL;i++) {
    //     //     printf("i = %d [%s], ",i,cleaned_argv[i]);
    //     // }
    //     // execvp(stripped_argv[0], stripped_argv);                  // execute in parent
    //     // fprintf(stderr, "error in child execi \n"); // error
    //     // exit(0);
    //     // char *x[] = {"sleep","5",NULL};
    //     // execlp("ls","ls",(char*)NULL);
    //     if (execvp(stripped_argv[0], stripped_argv)==-1) {  
    //         printf("*** ERROR: exec failed\n");
    //         exit(1);
    //     }
    //  }
    //  else {
    //     // pause();
    //     // Parent process    
    //     // printf("[%d] %d\n",jobId,pid); 
    //     if(is_waiting){
    //         // sigaction(SIGCHLD, &action, NULL);
    //         printf("[%d] %d\n",++jobId,pid); 
    //         // is_waiting = 0;
    //         // printf("\nParent detects process %d was done\n", pid);
    //         // while (wait(&status) != pid);
    //         return;
    //     }    
    //     else {
    //         while (wait(&status) != pid);
    //     }
    //     // else {
    //     //     // pause();
    //     // // printf("%d\n",not_waiting);
    //     // // if(!not_waiting){
    //     //     // printf("hello\n");     
    //     // //     return;
    //     // // }else{                 
    //     //     // printf("done"); 
    //     //     while (wait(&status) != pid);
    //     // }
    //     // }
    //  }
}
// void proc_exit()
// {
// 		int wstat;
// 		// union wait wstat;
// 		pid_t	pid;

// 		while (1) {
// 			pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
// 			if (pid == 0)
// 				return;
// 			else if (pid == -1)
// 				return;
// 			else{
// 				printf ("Return code: \n");
//                 return;
//             }
// 		}
// }

//  void handler(int sig)
// {
//   pid_t pid;

//   pid = wait(NULL);

//   printf("Pid %d exited.\n", pid);
// }

void read_line(char *line) {
    char *argv[64];
    parse(line, argv);

    // for(int i =0;argv[i] != NULL;i++) {
    //     printf("%s " , argv[i]);
    // }
    // printf("%s\n",line);
    // char *tmp;
    // strcpy(tmp,line);
    // char *token = strtok(line, " ");
    // // printf("%s\n",tmp);
    // char **arr = malloc(64 * sizeof(char*));;
    // int i = 0, index = 0;
    // while (token != NULL)
    // {

    //     // printf("%d\n", i);
    //     // if (i > max_len) {
    //         // printf("asdasda");
    //         // arr = malloc((5+1)*sizeof(char));
    //         // max_len++;
    //     arr[index++] = strdup(token);

    //     // arr = (char **)realloc(arr, (len_count + 1) * sizeof(char *));
    //     // arr[len_count++] = strdup(token);
    //     // arr[len_count++] = token;
    //     // arr[i]=malloc(sizeof(char*));
    //     // strcpy(arr[len_count], token);
    //     // len_count++;
    //     // printf("%s ", token);
    //     // i++;
    //     // }
    //     token = strtok (NULL, " ");
    // }
    // printf("%d ", len_count);
    // arr[]
    // arr[index] = NULL;
    // int n = index;
    // while (arr[n++] != NULL){}
    // n--;
    // size_t n = sizeof(arr)/sizeof(arr[0]);
    // printf("%s", arr[1]);
    // printf("%d\n",n);
    // if (strcmp(argv[0],"echo")==0) {
    //     for(int i =1;argv[i] != NULL;i++) {
    //     printf("%s " , argv[i]);
    //     }
    //     printf("\n");
    //     // printf("%s", arr[n-1]);
    // }
    if (strcmp(argv[0],"exit")==0) {
        exit(atoi(argv[1]));
    } 
    else if (strcmp(argv[0],"!!")==0) {
        size_t n_last_command = strlen(last_command);
        last_command[--n_last_command] = '\0';
        printf("%s",last_command);
        char input[64];
        fgets(input,25,stdin);
        read_line(last_command);
        // printf("\n");
    }
    else {
        execute(argv);
        // char *x[3];
        // printf("arg_list[0]= %s\n", argv[0]);
        // for (int i = 0; argv[i] != NULL; i++)
        // //  for (int i = 0; i<3; i++)
        // {
        //     // x[i] = argv[i];
        //     printf("arg_list[%d] = [%s]\n", i, argv[i]);
        // }
        // fflush(stdout);
        // char* argument_list[] = {"ls", "-la", NULL};
        // for(int i =0;argv[i] != NULL;i++) {
        //     printf("%s " , argv[i]);
        // }
        // execvp(argv[0], argument_list);
    }
//         pid_t pid, wpid;
//         pid = fork();
//   if (pid == 0) {
//     // Child process
//     if (execvp(arr[0], arr) == -1) {
//       perror("lsh");
//     }
//     exit(EXIT_FAILURE);
//   } else if (pid < 0) {
//     // Error forking
//     perror("lsh");
//   } else {
//       printf("hello");
//     // Parent process
//     // do {
//     //   wpid = waitpid(pid, &status, WUNTRACED);
//     // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//   }
        // pid_t pid, wpid;
        // char* argument_list[] = {"ls", "-l", NULL};
        // printf("%s", arr[0]);
        // execvp(arr[0],arr);
        // try();
    //     char* command = "ls";
 
    // printf("Before calling execvp()\n");
 
    // Calling the execvp() system call
    // int status_code = execvp(command, argument_list);
 
    // if (status_code == -1) {
    //     printf("Process did not terminate correctly\n");
    //     exit(1);
    // }
 
    // printf("This line will not be printed if execvp() runs correctly\n");
        // pid = fork();
        // printf("%d", pid);
        // pid = fork();
        // char *argv[n+1];
        // for (int i=0;i<n;i++){
        //     argv[i] = arr[i];
        //     // printf("%s ",arr[i]);
        // }
        // argv[n] = NULL;
        // for (int i=0;i<n;i++){
        // //     argv[i] = arr[i];
        //     printf("%s ",argv[i]);
        // }
    //     char* commandx = "ls";
    // char* argument_list[3] = {"ls", "-l", NULL};
        // printf("%s ", argv[0]);
        // printf("%s ",commandx);
    // printf("Before calling execvp()\n");
 
    // Calling the execvp() system call
    // int status_code = execvp(commandx, argument_list);
 
    // if (status_code == -1) {
    //     printf("Process did not terminate correctly\n");
    //     exit(1);
    // }
 
    // printf("This line will not be printed if execvp() runs correctly\n");
        // char* argument_list[] = {"ls", "-l", NULL}; // NULL terminated array of char* strings
        // try();
// Ok! Will execute the command "ls -l"
// execvp("ls", argument_list);
        // argv[n+1] = NULL;
        // execvp("ls", argv);
                // printf("%d\n", execvp("ls", argv));
        // for (int i=0;i<n+1;i++){
        //     printf("%s ",argv[i]);
        //     // printf("%s ",arr[i]);
        // }
    // if (pid == 0) {
    //     // Child process
    //     printf("Hello");
    //     if (execvp(arr[0], arr) == -1) {
    //     perror("lsh");
    //     }
    //     exit(EXIT_FAILURE);
    // } else if (pid < 0) {
    //     // Error forking
    //     perror("lsh");
    // } else {
    //     // Parent process
    //     // do {
    //     // wpid = waitpid(pid, &status, WUNTRACED);
    //     // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    // }
        // int out = system(arr[0]);
        // printf("%s\n", line);
        // system(tmp);
        //     printf("Command processor exists");
        // else
        //     printf("Command processor doesn't exists");
        // printf("%d",out);
        // printf("bad command\n");
    // }
    // printf("%zu\n", n);
    // printf("%d\n",n);
    // for (i = 0; i < n; ++i) 
    //     printf("%s\n", arr[i]);
    // printf("%lu",strlen(arr));
//    printf("%s", token);
}
// void handle_sigint(int sig)
// {
//     printf(" Caught signal %d\n", sig);
// }
void sigint_handler(int signal) {
    printf("\n");
}
void start_shell(){
    printf("Starting IC shell\n");
    jobId = 0;
    // struct sigaction sigint_action = malloc(sizeof sigaction);
    // struct sigaction sigint_action;
    // // {0
    // //     // .sa_handler = &sigint_handler,
    // //     // .sa_flags = 0
    // // };
    // pid_t pid = getpid();
    // setpgid(pid, pid);
    // tcsetpgrp(0, pid);
    // // sigemptyset(&sigint_action.sa_mask);
    // sigaction(SIGINT, &sigint_action, NULL);
    // signal(SIGCHLD, child_handler);
    // signal(SIGQUIT, SIG_IGN);
    // signal(SIGTSTP, SIG_IGN);
    // signal(SIGTTIN, SIG_IGN);
    // struct sigaction action = {0};
    // action.sa_handler = child_handler;
    // sigemptyset(&action.sa_mask);
    // action.sa_flags = 0;
    // sigaction(SIGCHLD, &action, NULL);
    // is_waiting = 0;
    // conversion [] = {

    // }
    // char instruction[1][10] = {
    //     "echo"
    // };
    // char x[64];
    while(1){
        // signal(SIGINT, handle_sigint);
        printf("icsh $ ");
        // signal(SIGINT, handle_sigint);
        char *command_line;
        size_t buffsize = 0;
        // child_handler();
        getline(&command_line, &buffsize, stdin);
        // command_line[strlen(command_line)-1]='\0';
        char tmp_command[64];
        strcpy(tmp_command, command_line);
        read_line(command_line);
        strcpy(last_command, tmp_command);
    };
}
void read_file(FILE *file){
    char buffer[MAX_LEN];
    while (fgets(buffer, MAX_LEN - 1, file) != NULL)  {
        read_line(buffer);
    }
}
// void handle(int sig){
//     printf("hello\n");
// }
// void handler(int sig)
// {
//   pid_t pid;

//   pid = wait(NULL);

//   printf("Pid %d exited.\n", pid);
// }
int main(int argc, char **argv){
//    signal(SIGCHLD, handler);
    // signal(SIGTTIN, SIG_IGN);
    // Have argument(s)
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
        // printf("argv[%d]: %s\n", i, argv[i]);
        FILE* fp;
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("Failed: ");
            return 1;
        }
        read_file(fp);
        }
    }
    else {
        // signal(SIGINT,handle);
        start_shell();
    }
    return 0;
}