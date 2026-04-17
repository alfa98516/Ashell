#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TOKENS 512

void tokenize(char* cmd) {
    Token* tokenArray = malloc(sizeof(Token) * MAX_TOKENS);
    printf("it runs !\n");
    Token curr = {UNINIT, ""};
    int i = 0; // index into lexeme.
    int j = 0; // index into tokenArray.
    for (int c = 0; cmd[c] != '\0' && j < MAX_TOKENS;
         c++) { // dont increment every time, we want to control consumption
        if (isspace(cmd[c])) {

            continue;
        }
        if (cmd[c] == '-' || curr.tokenId == FLAG) {
            curr.lexeme[i] = c;
            curr.tokenId = FLAG;
        }
    }
}
