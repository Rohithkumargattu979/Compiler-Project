/* 
GROUP NO: 30
ROHITH KUMAR GATTU - 2019A7PS0049H
ROHAN RAO NALLANI - 2019A7PS0048H
KASINA SATWIK - 2019A7PS0011H
SRIKAR SASHANK M - 2019A7PS0160H
*/

#ifndef LEXER_H
#define LEXER_H
#include<bits/stdc++.h>
FILE *getStream(FILE* fp, char* buffer, int bsize);

token getNextToken(FILE *fp, char* buffer, int bsize);

void createLexerHashTable();
long long hashFunction(string string);
int lookupFunction(string lexeme);

void removeComments(FILE* testfile);

int getLexErrors();
void resetLexErrors();
#endif



