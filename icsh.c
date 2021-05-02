#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 256
char last_command[64];
void read_line(char *line) {
    char *token = strtok(line, " ");
    char **arr = NULL;
    int i = 0, len_count = 0;
    while (token != NULL)
    {

        // printf("%d\n", i);
        // if (i > max_len) {
            // printf("asdasda");
            // arr = malloc((5+1)*sizeof(char));
            // max_len++;
        arr = (char **)realloc(arr, (len_count + 1) * sizeof(char *));
        // arr[len_count++] = strdup(token);
        arr[len_count++] = token;
        // arr[i]=malloc(sizeof(char*));
        // strcpy(arr[len_count], token);
        // len_count++;
        // printf("%s ", token);
        // i++;
        // }
        token = strtok (NULL, " ");
    }
    int n = 0;
    while (arr[n++] != NULL){}
    n--;
    // size_t n = sizeof(arr)/sizeof(arr[0]);
    // printf("%s", arr[1]);
    // printf("%d\n",n);
    if (strcmp(arr[0],"echo")==0) {
        for (i = 1; i < n-1; ++i) {
            printf("%s ", arr[i]);
        }
        printf("%s", arr[n-1]);
    }
    else if (strcmp(arr[0],"exit")==0) {
        exit(atoi(arr[1]));
    } 
    else if (strcmp(arr[0],"!!\n")==0) {
        size_t n_last_command = strlen(last_command);
        last_command[--n_last_command] = '\0';
        printf("%s",last_command);
        char input[64];
        fgets(input,25,stdin);
        read_line(last_command);
        printf("\n");
    }
    else {
        printf("bad command\n");
    }
    // printf("%zu\n", n);
    // printf("%d\n",n);
    // for (i = 0; i < n; ++i) 
    //     printf("%s\n", arr[i]);
    // printf("%lu",strlen(arr));
//    printf("%s", token);
}

void start_shell(){
    printf("Starting IC shell\n");
    // conversion [] = {

    // }
    // char instruction[1][10] = {
    //     "echo"
    // };
    // char x[64];
    while(1){
        printf("icsh $ ");
        char command_line[64];
    // scanf("%[^\n]",command_line);
        fgets(command_line,100,stdin);
        // scanf("%s",x);
        // char command_line[64];
        // fgets(x,25,stdin);
        // char *a = strrchr(x, '.');
        // scanf("%[^\n]",x);
        // printf("%s",a);
        // printf("%s",last_command);
        char tmp_command[64];
        strcpy(tmp_command, command_line);
        read_line(command_line);
        // printf("AFTER\n");
        strcpy(last_command, tmp_command);
        // printf("%s",last_command);
        // fgets(command, sizeof command, stdin);
        // switch (echo) {
        // if (strcmp(command,"echo")) {
        //     char argument[64];
        //     fgets(argument, sizeof argument, stdin);
        //     printf("%s",argument);
        // }
        // }
        // printf("%s", command);
        // scanf("");
    };
}
void read_file(FILE *file){
    char buffer[MAX_LEN];
    while (fgets(buffer, MAX_LEN - 1, file) != NULL)  {
        read_line(buffer);
    }
}

int main(int argc, char **argv){
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
        start_shell();
    }
    return 0;
}