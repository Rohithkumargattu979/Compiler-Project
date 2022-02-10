/* 
GROUP NO: 30
ROHITH KUMAR GATTU - 2019A7PS0049H
ROHAN RAO NALLANI - 2019A7PS0048H
KASINA SATWIK - 2019A7PS0011H
SRIKAR SASHANK M - 2019A7PS0160H
*/

#include<stdio.h>
#include<stdlib.h>
#include<lexerDef.h>

FILE *getStream(FILE* fp, char* buffer, int bsize);

token getNextToken(FILE *fp, char* buffer, int bsize);

void createLexerHashTable();
long long hashFunction(char* string);
int lookupFunction(char* lexeme);

void removeComments(File* testfile);

int getLexErrors();
void resetLexErrors();


