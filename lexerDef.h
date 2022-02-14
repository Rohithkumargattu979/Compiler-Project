/* 
GROUP NO: 30
ROHITH KUMAR GATTU - 2019A7PS0049H
ROHAN RAO NALLANI - 2019A7PS0048H
KASINA SATWIK - 2019A7PS0011H
SRIKAR SASHANK M - 2019A7PS0160H
*/

#ifndef LEXERDEF_INCLUDED
#define LEXERDEF_INCLUDED

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