#include<bits/stdc++.h>
using namespace std;

typedef struct{
    string identifier;
    string scope;
    string type;
    int lineNo;
} variable;

typedef struct{
    string name;
    string lineNo;
    vector<variable> funct_variable_list;
} funct;

typedef struct{
    vector<variable> variable_list;
    vector<funct> funct_list;

} symbolTable;