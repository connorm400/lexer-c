//
// Created by conno on 2024-01-15.
//

#ifndef LEXER_LEX_H
#define LEXER_LEX_H
#include <stdint.h>
#include <stdbool.h>
typedef struct lexer {
    const char* input;
    uint32_t position;
    uint32_t readPosition;
    char ch;
} lexer;

typedef enum tokenType {
    TOK_ILLEGAL, // illegal symbol
    TOK_EOF, // end of file
    TOK_IDENT, // identifier / symbol
    TOK_INT, // integer / number
    TOK_ASSIGN, // = equal sign
    TOK_PLUS, // +
    TOK_COMMA, // ,
    TOK_SEMICOLON, // ;

    TOK_LPAREN, // (
    TOK_RPAREN, // )
    TOK_LBRACE, // {
    TOK_RBRACE, // }

    TOK_FUNCTION, // fn
    TOK_LET,
} TokenType;

typedef struct token {
    enum tokenType type;
    char* literal;
} token;

char* showTokenType(int token);
lexer* lexer_new(char* input);
void lexer_free(lexer* l);
void lexer_readChar(lexer* l);
void lexer_skipWhitespace(lexer* l);
char* lexer_readIdentifier(lexer* l);
char* lexer_readInteger(lexer* l);
token* lexer_nextToken(lexer* l);
token* token_new(enum tokenType token, char* str);
void token_free(token* t);
bool isTokenLetter(char c);
enum tokenType lookupIdent(char* str);
#endif //LEXER_LEX_H
