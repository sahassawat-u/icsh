#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARG_SIZE 1
// enum instruction{ echo, !! };

// enum instruction get_enum_value(char * val) {
//     static char const * str[] = { "echo"};
//     for (int i = 0; i < echo; ++i)
//         if (!strcmp(str[i], val))
//             return i;
//     return END;
//  }
void read_line() {
    char command_line[64];
    // scanf("%[^\n]",command_line);
    fgets(command_line,25,stdin);
    char *token = strtok(command_line, " ");
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
    } else {
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
        // scanf("%s",x);
        // char command_line[64];
        // fgets(x,25,stdin);
        // scanf("%[^\n]",x);
        // printf("%s",x);
        read_line();
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

int main(){
    start_shell();
    return 0;
}