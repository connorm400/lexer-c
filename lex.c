//
// Created by conno on 2024-01-15.
//
#include "lex.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define CHECK_MALLOC(ptrname) \
    if (ptrname == NULL) {    \
        fprintf(stderr, "buy more ram"); \
        abort();              \
    }


char* showTokenType(int token) {
    switch (token) {
        case TOK_ILLEGAL:
            return "illegal";
        case TOK_EOF:
            return "end of file";
        case TOK_IDENT:
            return "identifier";
        case TOK_INT:
            return "integer";
        case TOK_ASSIGN:
            return "assignment operator";
        case TOK_PLUS:
            return "plus sign";
        case TOK_COMMA:
            return "comma";
        case TOK_SEMICOLON:
            return "semicolon";
        case TOK_LPAREN:
            return "left parenthesis";
        case TOK_RPAREN:
            return "right parenthesis";
        case TOK_LBRACE:
            return "left curly brace";
        case TOK_RBRACE:
            return "right curly brace";
        case TOK_FUNCTION:
            return "function";
        case TOK_LET:
            return "let binding";
        default:
            return "unknown";
    }
}

lexer* lexer_new(char* input) {
    lexer* new = malloc(sizeof(lexer));
    CHECK_MALLOC(new);

    new->input = malloc((strlen(input) + 1) * sizeof(char));
    CHECK_MALLOC(new->input);
    strcpy(new->input, input);

    new->readPosition = 0;
    new->position = 0;
    new->ch = '\0';

    lexer_readChar(new);

    return new;
}

void lexer_free(lexer* l) {
    free(l->input);
    free(l);
}

void lexer_readChar(lexer* l) {
    if (l->readPosition >= strlen(l->input)) {
        l->ch = '\0';
    } else {
        l->ch = l->input[l->readPosition];
    }
    l->position = l->readPosition;
    l->readPosition += 1;
}

token* token_new(enum tokenType tok, char* str) {
    token* new = malloc(sizeof(token));
    CHECK_MALLOC(new);

    new->type = tok;

    new->literal = malloc((strlen(str) + 1) * sizeof(char));
    CHECK_MALLOC(new->literal);
    strcpy(new->literal, str);

    return new;
}

token* lexer_nextToken(lexer* l) {
    token* tok;

    lexer_skipWhitespace(l);

    switch (l->ch) {
        case '=':
            tok = token_new(TOK_ASSIGN, "=");
            break;
        case ';':
            tok = token_new(TOK_SEMICOLON, ";");
            break;
        case ',':
            tok = token_new(TOK_COMMA, ",");
            break;
        case '+':
            tok = token_new(TOK_PLUS, "+");
            break;
        case '(':
            tok = token_new(TOK_LPAREN, "(");
            break;
        case ')':
            tok = token_new(TOK_RPAREN, ")");
            break;
        case '{':
            tok = token_new(TOK_LBRACE, "{");
            break;
        case '}':
            tok = token_new(TOK_RBRACE, "}");
            break;
        case '\0':
            tok = token_new(TOK_EOF, "");
            break;
        default:
            // all the other cases
            if (isTokenLetter(l->ch)) {
                char* keyword = lexer_readIdentifier(l);

                tok = token_new(lookupIdent(keyword), keyword);
                free(keyword);
                // we need to skip the lexer_readChar() that happens later
                return tok;
            } else if (isdigit(l->ch)) {
                char* integer = lexer_readInteger(l);
                tok = token_new(TOK_INT, integer);
                free(integer);
                return tok;
            } else {
                char* str = malloc(2 * sizeof(char));
                sprintf(str, "%c", l->ch);
                tok = token_new(TOK_ILLEGAL, str);
                free(str);
            }

            break;
    }
    lexer_readChar(l);
    return tok;
}

bool isTokenLetter(char c) {
    return isalpha(c) || c == '_' || c == '?' || c == '!';
}

char* lexer_readIdentifier(lexer* l) {
    uint32_t position = l->position;


    while (isTokenLetter(l->ch)) {
        lexer_readChar(l);
    }

    // because of that I need to add an extra plus one at the end of everything
    char* newstr = malloc((l->position - position + 1) * sizeof(char));
    CHECK_MALLOC(newstr);
    memmove(newstr, &l->input[position], (l->position - position) * sizeof(char));
    newstr[l->position - position] = '\0';
    return newstr;
}

char* lexer_readInteger(lexer* l) {
    uint32_t position = l->position;

    while (isdigit(l->ch)) {
        lexer_readChar(l);
    }

    char* newstr = malloc((l->position - position + 1) * sizeof(char));
    CHECK_MALLOC(newstr);
    memmove(newstr, &l->input[position], (l->position - position) * sizeof(char));
    newstr[l->position - position] = '\0';
    return newstr;
}

void token_free(token* t) {
    free(t->literal);
    free(t);
}

enum tokenType lookupIdent(char* str) {
    if (strcmp(str, "fn") == 0) {
        return TOK_FUNCTION;
    } else if (strcmp(str, "let") == 0) {
        return TOK_LET;
    } else {
        return TOK_IDENT;
    }
}

void lexer_skipWhitespace(lexer* l) {
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r')
        lexer_readChar(l);
}