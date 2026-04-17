#include "dir.h"
#include "parser.h"

#include <fcntl.h>
#include <pty.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 255
#define MAX_JOBS 32
#define e(err, ...)                                                                                \
    if (err == -1) {                                                                               \
        fprintf(stderr, __VA_ARGS__);                                                              \
    }
#define cmp(s1, s2)                                                                                \
    strcmp(s1, s2) == 0 // chud ass strcmp, stupid fucking function, why did they make it like this
// its more that im just annoyed i cant do "s1 == s2"

char* JOBS_LIST[MAX_JOBS];

int assignJob(char* cmd) {
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (JOBS_LIST[i] == NULL) {
            JOBS_LIST[i] = cmd;
            return i;
        }
    }
    return -1;
}

// HACK: This whole function is a hack, i should really be checking for
int isInteractive(char* cmd) {
    char* interactive[] = {"vim", "vi", "nvim", "top", "htop", "less", "man",
                           "ssh", "sh", "bash", "zsh", "sudo", NULL};
    for (int i = 0; interactive[i] != NULL; ++i) {
        if (cmp(cmd, interactive[i])) return 1;
    }
    return 0;
}

int convertToV(char* s, char** args) {

    if (s == NULL) {
        return -1;
    }
    if (args == NULL) {
        return -1;
    }

    // HACK: also lowkey a hack, should just parse and lex here.
    char* token = strtok(s, " ");

    int j = 0;
    while (token != NULL && j < MAX_ARGS - 1) {
        args[j] = token;
        token = strtok(NULL, " ");
        ++j;
    }

    args[j] = NULL;

    return 0;
}

// HACK: once agin, should be moved to a parser
int backgroundProcess(char* command) {
    for (int i = 0; command[i] != '\0'; ++i) {
        if (command[i] == '&' && command[i + 1] == '\0') return 1;
    }
    return 0;
}

/*
 * @returns: 1 if it should exit, 0 if it should keep running, -1 if an error occured
 */
int prompt() {
    pid_t PARENT_PID;
    printf("\e[0;32m$ \e[0m");
    char s[MAX_ARG_NAME + 1];
    char out[4096];
    fgets(s, sizeof(s), stdin);

    int master_fd;
    struct winsize ws = {24, 80, 0, 0};

    if (s[0] == '\n') return 0; // no command was given

    size_t size = strlen(s);

    s[strcspn(s, "\n")] = 0; // remove newline

    int isbg = backgroundProcess(s);
    if (isbg) {
        s[size - 2] = '\0';
        assignJob(s);
    }

    if (cmp("exit", s)) {
        return 1;
    }
    char** args = (char**)malloc(MAX_ARGS * sizeof(char*));

    int err = convertToV(s, args);

    e(err, "converting to vector failed");
    if (isInteractive(args[0])) { // give full control of terminal if its an interceptive command
        if ((PARENT_PID = fork()) == 0) {
            err = execvp(args[0], args);
            e(err, "%s: Not a recognized command\n",
              args[0]); // probably actually neve gonna happen since its verified as an interactive
                        // command
            exit(0);

        } else if (!isbg) {
            wait(NULL);
        }
    } else {
        if ((PARENT_PID = forkpty(&master_fd, NULL, NULL, &ws)) == 0) {
            err = execvp(args[0], args);
            e(err, "%s: Not a recognized command\n", args[0]);
            exit(0);
        } else if (!isbg) { // wait for child if its not a background proccess
            wait(NULL);
            int nbytes = 0, n;
            while ((n = read(master_fd, out + nbytes, sizeof(out) - nbytes - 1)) > 0) {
                nbytes += n;
            }
            out[nbytes] = '\0';
            printf("%s", out);
            close(master_fd);
        }
    }
    free(args);
    return 0;
}

int main() {
    int eCode = 0;
    while (eCode != 1 && eCode != -1) {
        eCode = prompt();
    }
}
