

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include<stdio.h>

typedef struct{
    int tokenId;
    char* value;
    char* tokenString;
    int line;
} token;
// token that should be generated
typedef struct{
    char* keyWord;
    int tokenId;
} hashTable; // hashtable attributes


int state;
int offset;
int lexicalError;
int lineNo;

#endif