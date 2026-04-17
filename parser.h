
#define MAX_ARG_NAME 255
typedef enum { CMD, CD, AMPERSAND, PIPE, FLAG, UNINIT } TokenId;

typedef struct {
    TokenId tokenId;
    char lexeme[255];
} Token;

void tokenize(char* cmd);
