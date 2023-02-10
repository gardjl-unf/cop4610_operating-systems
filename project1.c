#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_BUFFER 256
#define RESPONSE_BUFFER 65536
#define SPACE " "
#define TERMINATOR '\0'
#define NEWLINE '\n'
#define PROMPT "Valid Commands: cd, dir, type, del, ren, copy\n"
#define EXIT_STRING "Type Ctrl-C to exit\n"
#define INVALID_COMMAND "Invalid command. Please try again.\n"
#define CD_COMMAND_W "cd"
#define CD_COMMAND_L "cd "
#define DIR_COMMAND "dir"
#define LS_COMMAND "ls -l"
#define TYPE_COMMAND "type"
#define CAT_COMMAND "cat "
#define DEL_COMMAND "del"
#define RM_COMMAND "rm "
#define REN_COMMAND "ren"
#define MV_COMMAND "mv "
#define COPY_COMMAND "copy"
#define CP_COMMAND "cp "
#define PWD_COMMAND "pwd"
#define POPEN_MODE "r"
#define STR_F "%s"
#define TOK_F " \t\n\r"
#define DIV "> "

typedef struct {
    char input[INPUT_BUFFER];
    char command[INPUT_BUFFER];
    char arg1[INPUT_BUFFER];
    char arg2[INPUT_BUFFER];
    char output[INPUT_BUFFER];
    char response[RESPONSE_BUFFER];
} COMMAND;

void prompt(COMMAND*);
void printr(COMMAND*);
void reset(COMMAND*);
void run(COMMAND*);

int main() {
    COMMAND cmd;
    char *token;

    printf(STR_F, PROMPT);
    printf(STR_F, EXIT_STRING);

    while (1) {
        prompt(&cmd);
        while (fgets(cmd.input, INPUT_BUFFER, stdin)) {
            if ((token = strtok(cmd.input, TOK_F))) {
                strcpy(cmd.command, token);
                if ((token = strtok(NULL, TOK_F))) {
                    strcpy(cmd.arg1, token);
                    if ((token = strtok(NULL, TOK_F))) {
                        strcpy(cmd.arg2, token);
                    }
                }   
            }
             
            if (strcmp(cmd.command, CD_COMMAND_W) == 0 && cmd.arg1 != NULL) {
                strcat(cmd.output, cmd.arg1);
                chdir(cmd.arg1);
            }
            else if (strcmp(cmd.command, DIR_COMMAND) == 0) {
                strcpy(cmd.output, LS_COMMAND);
                run(&cmd);
                printr(&cmd);
            }
            else if (strcmp(cmd.command, TYPE_COMMAND) == 0 && cmd.arg1 != NULL) {
                strcpy(cmd.output, CAT_COMMAND);
                strcat(cmd.output, cmd.arg1);
                run(&cmd);
                printr(&cmd);
            }
            else if (strcmp(cmd.command, DEL_COMMAND) == 0 && cmd.arg1 != NULL) {
                strcpy(cmd.output, RM_COMMAND);
                strcat(cmd.output, cmd.arg1);
                run(&cmd);
                printr(&cmd);
            }
            else if (strcmp(cmd.command, REN_COMMAND) == 0 && cmd.arg1 != NULL && cmd.arg2 != NULL) {
                strcpy(cmd.output, MV_COMMAND);
                strcat(cmd.output, cmd.arg1);
                strcat(cmd.output, SPACE);
                strcat(cmd.output, cmd.arg2);
                run(&cmd);
                printr(&cmd);
            }
            else if (strcmp(cmd.command, COPY_COMMAND) == 0 && cmd.arg1 != NULL && cmd.arg2 != NULL) {
                strcpy(cmd.output, CP_COMMAND);
                strcat(cmd.output, cmd.arg1);
                strcat(cmd.output, SPACE);
                strcat(cmd.output, cmd.arg2);
                run(&cmd);
                printr(&cmd);
            }
            else {
                printf(STR_F, INVALID_COMMAND);
            }
            prompt(&cmd);
        }
    }
}

void prompt(COMMAND *c) {
    reset(c);
    strcpy(c->output, PWD_COMMAND);
    run(c);
    *strchr(c->response, NEWLINE) = TERMINATOR;
    strcat(c->response, DIV);
    printr(c);
    reset(c);
}

void reset(COMMAND *c) {
    memset(c->input, '\0', INPUT_BUFFER * sizeof(char));
    memset(c->command, '\0', INPUT_BUFFER * sizeof(char));
    memset(c->arg1, '\0', INPUT_BUFFER * sizeof(char));
    memset(c->arg2, '\0', INPUT_BUFFER * sizeof(char));
    memset(c->output, '\0', INPUT_BUFFER * sizeof(char));
    memset(c->response, '\0', RESPONSE_BUFFER * sizeof(char));
}

void run(COMMAND *c) {
    FILE *fp;
    size_t responseLength;
    char response[RESPONSE_BUFFER];
    fp = popen(c->output, POPEN_MODE);
    if (fp != NULL) {
        responseLength = fread(response, sizeof(char), RESPONSE_BUFFER - 2, fp);
        if (response[0] == NEWLINE) {
            response[0] = TERMINATOR;
        }
        else {
            response[responseLength] = '\n';
            response[responseLength + 1] = '\0';
        }
        strcpy(c->response, response);
    }
    pclose(fp);
}

void printr(COMMAND *c) {
    printf(STR_F, c->response);
}