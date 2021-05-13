#include <unistd.h>

void  execute(char **argv, int jobId, int is_waiting)
{
    pid_t  pid;
    // struct sigaction action;
    // action.sa_handler = child_handler;
    // sigemptyset(&action.sa_mask);
    // action.sa_flags = 0;
    // sigaction(SIGCHLD, &action, NULL);
    int status;
    for(int i =0;argv[i]!=NULL;i++){
        if(!strcmp(argv[i], "&")) {
            // sigaction(SIGCHLD, &action, NULL);
            // printf("[%d] %d\n",++jobId,pid); 
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
            stripped_argv[idx++] = argv[j];
        } 
        stripped_argv[idx-1] = (char*)NULL;
    if (is_waiting) {
        int background_process_id;
        pid_t fork_return;
        fork_return = fork();

        if (fork_return == 0) {
            // printf("[%d] %d\n",++jobId,fork_return); 
            background_process_id = getpid();
            // addJobToTable(strippedCommand, background_process_id);
            setpgid(0, 0);
            execvp(stripped_argv[0], stripped_argv);
            // execve(executableCommands[0], executableCommands, NULL);
            exit(0);
        } else {
            return;
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
    //     int n = 0, in, out;
    //     while(argv[n]!=NULL)n++;
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
    //     char *stripped_argv[n];
    //     int idx = 0;
    //     for (int j = 0; j < n; j++) {
    //         if(!strcmp(argv[j], "&")) {
    //             ++j;
    //             // is_waiting = 1;   
    //         }
    //         // printf("%s %d\n",argv[j],j);
    //         if (!strcmp(argv[j], "<")) {        // looking for input character
    //             ++j;
    //             if ((in = open(argv[j-1], O_RDONLY)) < 0) {   // open file for reading
    //                 fprintf(stderr, "error opening file\n");
    //             }
    //             // j+=3;
    //             dup2(in, STDIN_FILENO);         // duplicate stdin to input file
    //             close(in);                      // close after use
    //             continue;
    //         }                                   // end input chech

    //         if (!strcmp(argv[j], ">")) {        // looking for output character
    //             ++j;
    //             // printf("hello\n");
    //             // printf("%d\n",j);
    //             // out = creat(argv[j], 0644); // create new output file
    //             out = open(argv[j++], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //             // // int output_fds = open("./output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //             // // out =  open(argv[j], O_TRUNC | O_CREAT | O_WRONLY, 0666);
    //             dup2(out, STDOUT_FILENO);       // redirect stdout to file
    //             close(out);                     // close after usere
    //             // printf("dup2 = %d and cloase = %d\n",,);
    //             continue;
    //         }                                   // end output check
    //         // if (!strcmp(argv[j],"&")) {
    //         //     ++j;
    //         //     jobId++;
    //         //     printf("[%d] %d",jobId,pid);
    //         // }
    //         // if (!strcmp(argv[j], ">>")) {       // looking for append
    //         //     ++j;
    //         //     int append = open(argv[j], O_CREAT | O_RDWR | O_APPEND, 0644);

    //         //     dup2(append, STDOUT_FILENO);
    //         //     close(append);
    //         //     continue;
    //         // }

    //         stripped_argv[idx++] = argv[j];
    //     } 
    //     // printf("%d\n", not_waiting);
    //     stripped_argv[idx-1] = (char*)NULL;
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
    //     if(!is_waiting){
    //         // sigaction(SIGCHLD, &action, NULL);
    //         // printf("[%d] %d\n",++jobId,pid); 
    //         // is_waiting = 0;
    //         // printf("\nParent detects process %d was done\n", pid);
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