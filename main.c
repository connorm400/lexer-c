#include <stdio.h>
#include "lex.h"

int main() {
    char* input = "let five = 5;\n"
                  "let ten = 10;\n"
                  "let add = fn(x, y) {\n"
                  "  x + y;\n"
                  "};\n"
                  "\n"
                  "let result = add(five, ten);";

    printf("Lexing: \"%s\"\n===\n", input);
    lexer* lex = lexer_new(input);

    while (1) {
        token* currentToken = lexer_nextToken(lex);
        if (currentToken->type == TOK_EOF) {
            printf("==end of file==\n");
            break;
        } else {
            printf("%s: \"%s\"\n", showTokenType(currentToken->type), currentToken->literal);
        }

        token_free(currentToken);
    }

    lexer_free(lex);
    return 0;
}
