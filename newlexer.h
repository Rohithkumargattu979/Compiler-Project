#include<bits/stdc++.h>
#include<fstream>
#include<iostream>
using namespace std;
typedef struct{
    int tokenId;
    string value;
    string tokenString;
    int line;
} token;
typedef struct{
    string identifier;
    string scope;
    string type;
    int lineNo;
    token token;
    int len;
}Array;
// token that should be generated
typedef struct{
    string keyWord;
    int tokenId;
} hashTable; // hashtable attributes

typedef struct{
    string identifier;
    string scope;
    string type;
    int lineNo;
    token token;
} variable;

typedef struct{
    int specialId;
    string name;
    string lineNo;
    token token;
    vector<variable> funct_variable_list;
    vector<Array> funct_array_list;
    map<string,variable> funct_variable_map;
    map<string,Array> funct_array_map;
} funct; // assumption : atmost 1000 variables will only be present in any function

typedef struct{
    vector<variable> variable_list;
    vector<funct> funct_list;
    map<string,funct> funct_map;
    vector<Array> array_list;
} symbolTable;




int state;
int offset = 0;
int lexicalError;
int lineNo;
int funcToken = 10000;
int specialfunid = 10001;
string PREV_FUN_NAME = "Global";
string CUR_FUN_NAME;
void createLexerHashTable();
long long hashFunction(string string);
int lookupFunction(string lexeme);

void removeComments(FILE* testfile);

int getLexErrors();
void resetLexErrors();
string getCurFun();
string updateCurFun();
token getNextToken(vector<char>& b);
void printTokenList();
vector<char> getInputStream(string s);
string removeAllComments(string filename);
