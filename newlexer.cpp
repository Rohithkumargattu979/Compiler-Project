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


symbolTable st;

stack<string> functStack;

int state;
int offset = 0;
int lexicalError;
int lineNo;
int funcToken = 10000;
int specialfunid = 10001;

void createLexerHashTable();
long long hashFunction(string string);
int lookupFunction(string lexeme);

void removeComments(FILE* testfile);

int getLexErrors();
void resetLexErrors();
string PREV_FUN_NAME = "Global";
string CUR_FUN_NAME;
// ------------------------------------------------------------------------------------------------- //
map<int,hashTable> ht;
long long hashFunc(string str){
    long long p=31;
    long long mod=1e2+9;
    long long ans=0;
    for(int i=0;i<str.size();i++){
        ans+=((str[i]*p)%mod);
        p=p*p;
        p%=mod;
    }
    return ans;
}
int lookup(string lexeme){

	int in=hashFunc(lexeme);
	if(ht.find(in) != ht.end() && (ht[in].keyWord == lexeme)){

		return ht[in].tokenId;
	}
	return -1;

}
void createLexerHashTable(){
	const int m=1e2+9;
	
	char keywords[11][15]={"Int","Float","If","Else","For","While",
	"Return","String","Void","Array", "main"}; //add few keywords if needed

	int tokenids[11]={1,2,3,4,5,6,7,8,9,10,11};

	for(int j=0;j<11;j++){
        hashTable t;
        
        t.keyWord = keywords[j];
		t.tokenId=tokenids[j];
        // cout<<hashFunc(keywords[j])<<endl;
        ht[hashFunc(keywords[j])]=t;
        //  ht[hashFunc(keywords[j])].keyWord=keywords[j];
        //   ht[hashFunc(keywords[j])].tokenId=tokenids[j];
	}

}
string getCurFun(){
    return CUR_FUN_NAME;
}
void updateCurFun(string newName){
    CUR_FUN_NAME = newName;
}
bool checkMain(string funName){
    return funName == "main";
}
token createFuncToken(string funName){
    token ans;
    ans.tokenId = funcToken;
    ans.value = funName;
    ans.tokenString = "TK_FUNCT";
    funcToken += 1000;
    funct fun;
    fun.specialId = specialfunid;
    specialfunid += 1000;
    fun.token = ans;
    fun.name = funName;
    fun.lineNo=lineNo;
    st.funct_list.push_back(fun);
    functStack.push(funName);
    updateCurFun(funName);
    st.funct_map[funName] = fun;
    return ans;
}
token addMain(){
    token ans;
    string str="main";
    // cout<<"hello&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
    ans.tokenId= ht[hashFunc(str)].tokenId;
    ans.value= "mainFunc";
    ans.tokenString="TK_MAIN";
    funct fun;
    fun.token=ans;
    fun.lineNo=lineNo;
    fun.name=str;
    st.funct_list.push_back(fun);
    updateCurFun(str);
    st.funct_map[str]=fun;
    functStack.push(str);
    return ans;
}
token addGlobalFun(){
    functStack.push("Global");
    funct fun;
    fun.name = "Global";
    fun.specialId = specialfunid;
    specialfunid += 1000;
    token ans;
    ans.tokenId = funcToken;
    funcToken += 1000;
    ans.tokenString = "TK_GLOBALFUNC";
    ans.value = fun.name;
    st.funct_list.push_back(fun);
    st.funct_map[fun.name] = fun;
    return ans;
}
void updateAtEnd(){
    if(functStack.size() >1){
        functStack.pop();
    }
    CUR_FUN_NAME = functStack.top();

}
token checkFunc(string funName){
    token t;
    map<string,funct> mp = st.funct_map;
    // cout<<checkMain(funName)<<"#####################"<<endl;
    if(checkMain(funName)){
        if(mp.find(funName)==mp.end()){
            t=addMain();
            return t;
        }else{
            return mp[funName].token;
        }
    }
    
    if(mp.find(funName) == mp.end()){
       t = createFuncToken(funName);
    }
    else{
        t = mp[funName].token;
    }
    return t;
}
token createVariableToken(string newVariable){
    token ans;
    ans.tokenId = st.funct_map[getCurFun()].specialId;
    st.funct_map[getCurFun()].specialId++;
    ans.tokenString = "TK_VARIABLE";
    ans.value = newVariable;
    return ans;
}
variable createNewVariable(string newVariable){
    string curfun = getCurFun();
    funct fun = st.funct_map[curfun];
    map<string,variable> mp = fun.funct_variable_map;
    variable ans;
    if(mp.find(newVariable) == mp.end()){
        ans.identifier = newVariable;
        ans.scope = getCurFun();
        ans.lineNo = lineNo;
        ans.token = createFuncToken(newVariable);
        fun.funct_variable_map[newVariable] = ans;
        fun.funct_variable_list.push_back(ans);
    }
    else{
        ans = mp[newVariable];
    }
    return ans;
    
    
}

token addVariable(string newVariable){
    string curfun = getCurFun();
    funct fun = st.funct_map[curfun];
    variable var = createNewVariable(newVariable);
    return var.token;

}

token createArrayToken(string newArray){
    token ans;
    ans.tokenId = st.funct_map[getCurFun()].specialId;
    st.funct_map[getCurFun()].specialId++;
    ans.tokenString = "TK_ARRAY";
    ans.value = newArray;
    
    return ans;
}
Array createNewArray(string newArray){
    string curfun = getCurFun();
    funct fun = st.funct_map[curfun];
    map<string,Array> mp = fun.funct_array_map;
    Array ans;
    if(mp.find(newArray) == mp.end()){
        ans.identifier = newArray;
        ans.scope = getCurFun();
        ans.lineNo = lineNo;
        // ans.len = l;
        ans.token = createFuncToken(newArray);
        fun.funct_array_map[newArray] = ans;
        fun.funct_array_list.push_back(ans);
    }
    else{
        ans = mp[newArray];
    }
    return ans;
    
    
}
// token addArray(string newArray){
//     string curfun = getCurFun();
//     funct fun = st.funct_map[curfun];
//     Array arr = createNewArray(newArray);
//     return arr.token;

// }
// ------------------------------------------------------------------------- //
// we should add extra params 
token getNextLexeme(vector<char>& buffer){ 
	state = 1;
    string lexeme;
	int i = 0;
	token token;
	token.value = "";
	token.tokenId=0;
    token.tokenString = "";

    while(true){
        if(offset >= buffer.size()){
            token.value = "@";
            token.tokenId = 420;
            token.tokenString = "TK_EOF";
            return token;
        }
        //printf("hello");
        if(buffer[offset]=='\t'||buffer[offset]==' '||buffer[offset]=='\n'){
           if(buffer[offset]=='\n'){
            lineNo++;
            }
            offset++;
           state=1;
           token.tokenId=22;//note this
           token.value= "&";
           token.tokenString="TK_SPACE";
           return token;
        }
        if(buffer[offset]==';'){
            offset++;
            state=2;
            token.value=";";
            token.tokenString="TK_SEMICOLON";
            token.tokenId=401;
            return token;
        }
        if(buffer[offset] == '&'){
            offset++;
            if(buffer[offset] == '&'){
                state = 4;
                token.value = "&&";
                token.tokenId = 51; 
                token.tokenString = "TK_LOGAND"; 
                offset++;
                return token;
            }
            else{
                state = 3;
                token.value = "&";
                token.tokenId = 50; 
                token.tokenString = "TK_AND"; 
                return token;
            }

        }
        else if(buffer[offset] == '|'){
            offset++;
            if(buffer[offset] == '|'){
                state = 6;
                token.value = "||";
                token.tokenId = 53;
                token.tokenString = "TK_LOGOR";
                offset++;
                return token;
            }
            else{
                state = 5;
                token.value = "|";
                token.tokenId = 52;
                token.tokenString = "TK_OR";
                return token;
            }
        }
        else if(buffer[offset] == '+'){
            offset++;
            state = 7;
            if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            string str1;
            char str2 = buffer[offset];
            str1 = str1 + str2;
            offset++;
            state = 8;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 8;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 9;
                char str3 = buffer[offset];
                offset++;
                fll += str3;
                while(buffer[offset] >= '0' && buffer[offset] <='9'){
                    str3 = buffer[offset];
                    fll += str3;
                    state = 124;
                    offset++;
                }
            }
            int MAX_FLOAT_LEN = 10;
            int MAX_INT_LEN = 8;
            if(flag == 0){
                if(str1.size() <= MAX_INT_LEN){ // len before '.'
                    token.tokenId = 73;
                    token.tokenString = "TK_INT";
                    token.value="+"+str1;
                    return token;
                }
                else{
                    ///// handle error
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TK_FLOAT";
                    token.value="+"+str1+fll;
                    return token;
                }
                else{
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            token.value =str1+fll; 
            return token;
        }
            token.value = "+";
            token.tokenId = 54;
            token.tokenString = "TK_ADD";
            return token;
        }
        else if(buffer[offset] == '-'){
            offset++;
            state = 10;
            if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            string str1;
            char str2 = buffer[offset];
            str1 = str1 + str2;
            offset++;
            state = 8;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 8;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 9;
                char str3 = buffer[offset];
                offset++;
                fll += str3;
                while(buffer[offset] >= '0' && buffer[offset] <='9'){
                    str3 = buffer[offset];
                    fll += str3;
                    state = 9;
                    offset++;
                }
            }
            int MAX_FLOAT_LEN = 10;
            int MAX_INT_LEN = 8;
            if(flag == 0){
                if(str1.size() <= MAX_INT_LEN){ // len before '.'
                    token.tokenId = 73;
                    token.tokenString = "TK_INT";
                    token.value="-"+str1;
                    return token;
                }
                else{
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TK_FLOAT";
                    token.value="-"+str1+fll;
                    return token;
                }
                else{
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            token.value =str1+fll; 
            return token;
        }
            token.value = "-";
            token.tokenId = 55;
            token.tokenString = "TK_MINUS";
            return token;
        }
        else if(buffer[offset] == '*'){
            offset++;
            state = 11;
            token.value = "*";
            token.tokenId = 56;
            token.tokenString = "TK_MUL";
            return token;
        }
        else if(buffer[offset] == '/'){
            offset++;
            state = 12;
            token.value = "/";
            token.tokenId = 57;
            token.tokenString = "TK_DIV";
            return token;
        }
        else if(buffer[offset] == '%'){
            offset++;
            state = 13;
            token.value = "%";
            token.tokenId = 58;
            token.tokenString = "TK_MOD";
            return token;
        }
        else if(buffer[offset] == '!'){
            offset++;
            if(buffer[offset] == '='){
                state = 15;
                token.value = "!=";
                token.tokenId = 60; 
                token.tokenString = "TK_NOTEQ"; 
                offset++;
                return token;
            }
            else{
                state = 14;
                token.value = "!";
                token.tokenId = 59; 
                token.tokenString = "TK_NOT"; 
                return token;
            }
        }
        else if(buffer[offset] == '>'){
            offset++;
            if(buffer[offset] == '='){
                state = 17;
                token.value = ">=";
                token.tokenId = 62; 
                token.tokenString = "TK_GEQ"; 
                offset++;
                return token;
            }
            else{
                state = 16;
                token.value = ">";
                token.tokenId = 61; 
                token.tokenString = "TK_GET"; 
                return token;
            }
        }
        else if(buffer[offset] == '<'){
            offset++;
            if(buffer[offset] == '='){ 
                state = 19;
                token.value = "<=";
                token.tokenId = 64; 
                token.tokenString = "TK_LEQ"; 
                offset++;
                return token;
            }
            else{
                state = 18;
                token.value = "<";
                token.tokenId = 63; 
                token.tokenString = "TK_LET"; 
                return token;
            }
        }
        else if(buffer[offset] == '='){
            offset++;
            if(buffer[offset] == '='){
                state = 20;
                token.value = "==";
                token.tokenId = 65; 
                token.tokenString = "TK_EQUALS"; 
                offset++;
                return token;
            }
            else{
                // error 
                cout<<"line:"<<lineNo<<":no such symbol exists, try changing to : or =="<<endl;
                token.tokenId = 999;
                token.tokenString = "TK_ERROR";
                token.value = "Syntax Error";
                return token;
            }
        }
        else if(buffer[offset] == ':'){
            offset++;
            state = 21;
            token.value = ":";
            token.tokenId = 66;
            token.tokenString = "TK_ASSIGN";
            return token;
        }
        else if(buffer[offset] == '{'){
            offset++;
            state = 22;
            token.value = "{";
            token.tokenId = 67;
            token.tokenString = "TK_FLOPEN";
            return token;
        }
        else if(buffer[offset] == '}'){
            offset++;
            state = 23;
            token.value = "}";
            token.tokenId = 68;
            token.tokenString = "TK_FLCLOSE";
            return token;
        }
        else if(buffer[offset] == '('){
            offset++;
            state = 24;
            token.value = "(";
            token.tokenId = 69;
            token.tokenString = "TK_SMOPEN";
            return token;
        }
        else if(buffer[offset] == ')'){
            offset++;
            state = 25;
            token.value = ")";
            token.tokenId = 70;
            token.tokenString = "TK_SMCLOSE";
            return token;
        }
        else if(buffer[offset] == '['){
            offset++;
            state = 26;
            token.value = "[";
            token.tokenId = 71;
            token.tokenString = "TK_SQOPEN";
            return token;
        }
        else if(buffer[offset] == ']'){
            offset++;
            state = 27;
            token.value = "]";
            token.tokenId = 72;
            token.tokenString = "TK_SQCLOSE";
            return token;
        }
        else if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            string str1;
            char str2 = buffer[offset];
            str1 = str1 + str2;
            offset++;
            state = 8;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 8;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 9;
                char str3 = buffer[offset];
                offset++;
                fll += str3;
                while(buffer[offset] >= '0' && buffer[offset] <='9'){
                    str3 = buffer[offset];
                    fll += str3;
                    state = 9;
                    offset++;
                }
            }
            int MAX_FLOAT_LEN = 10;
            int MAX_INT_LEN = 8;
            if(flag == 0){
                if(str1.size() <= MAX_INT_LEN){ // len before '.'
                    token.tokenId = 73;
                    token.tokenString = "TK_INT";
                    token.value=str1;
                    return token;
                }
                else{
                    ///// handle error
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TK_FLOAT";
                    token.value=str1+fll;
                    return token;
                }
                else{
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            token.value =str1+fll; 
            return token;
        }
        else if(buffer[offset]=='"'){
            state = 28;
            string str1;
            char str2 = buffer[offset];
            str1 += str2;
            offset++;
            while(buffer[offset]!='"'){
                str2=buffer[offset];
                str1 += str2;
                offset++;
            }
            state = 29 ;
            str1+=buffer[offset];
            offset++;
            //found a bug -now debugged
            // cout<<"hello"<<endl;
            token.value = str1;
            token.tokenId = 75;
            token.tokenString = "TK_STRING";
            // cout<<"hello2"<<endl;
            // cout<<token.tokenId<<" "<<token.tokenString<<endl;
            return token;
        }
        else if(buffer[offset] >= 'A' && buffer[offset] <= 'Z'){
            state = 30;
            string str;
            str += buffer[offset];
            offset++;
            while(buffer[offset] >= 'a' && buffer[offset] <= 'z'){
                str += buffer[offset];
                offset++;
                // cout<<"<hello"<<endl;
                // cout<<str<<"****"<<lookup(str)<<endl;
                if(lookup(str) != -1){
                    token.tokenId = ht[hashFunc(str)].tokenId;
                    token.value = str;
                    token.tokenString = "TK_" + ht[hashFunc(str)].keyWord;
                    return token;
                }
            }
            // handle error
            cout<<"no such keyword:"<<str<<"please check syntax"<<endl;
            //******we need to change this token to error token
            token.tokenId = 999;
            token.tokenString = "TK_ERROR";
            token.value = "Syntax Error";
            return token;
            //return token;
        }
        else if(buffer[offset] >= 'a' && buffer[offset] <='z'){
            state = 31;
            string str;
            str +=  buffer[offset];
            int flagalpha = 0;
            int flagnum = 0;
            offset++;
            int MAX_VARIABLE_LEN = 15;
            int lenalpha = 1;
            int len = 1;
            int lennum = 0;
            if(flagalpha == 0){
                while(buffer[offset] >='a' && buffer[offset] <='z'){ // min two alphas must be there to name a var
                    flagalpha = 1;
                    lenalpha++;
                    str += buffer[offset];
                    offset++;
                    len++;
                }
                if(flagalpha == 0){
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Variable should have atleast one alphabet in decleration"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            if(flagalpha == 1 && flagnum == 0){
                
                while(buffer[offset] >='0' && buffer[offset] <='9'){
                    state = 32;
                    lennum++;
                    flagnum = 1;
                    str += buffer[offset];
                    offset++;
                    len++;  
                    if(len > MAX_VARIABLE_LEN){
                        // handle error
                        // cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        // token.tokenId = 999;
                        // token.tokenString = "TK_ERROR";
                        // token.value = "Syntax Error";
                        // return token;
                    }
                }
                if(flagnum == 0){
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Variable should contain atleast one numerical to declare"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                else if(lenalpha > MAX_VARIABLE_LEN || lennum > MAX_VARIABLE_LEN || lenalpha + lennum > MAX_VARIABLE_LEN){
                        // handle error
                        cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        token.tokenId = 999;
                        token.tokenString = "TK_ERROR";
                        token.value = "Syntax Error";
                        return token;
                    }
                else{
                    token = createVariableToken(str);
                    return token;
                }
            }

        }
        else if(buffer[offset] == '@'){
            state = 33;
            offset++;
            string str = "";//not included $
            // cout<<str<<"@@@@"<<endl;
            while(buffer[offset] >= 'a' && buffer[offset] <='z'){
                 str += buffer[offset];
                offset++;
                // cout<<str<<"@@@@"<<endl;
               
            }
            // cout<<str<<"@@@@"<<endl;
            token =checkFunc(str);
            return token;
            
        }
        else if(buffer[offset] == '~'){
            offset++;
            state = 34;
            token.tokenId = 76;
            token.value = "~";
            token.tokenString = "TK_FUNCTEND";
            updateAtEnd();
            return token;
        }
        else if(buffer[offset] == '?'){
            state = 39;
            offset++;
            string str = "?";
            while(buffer[offset] >= 'a' && buffer[offset] <='z'){
                str += buffer[offset];
                offset++;
            }
            if(str == "?read"){
                state = 40;
                token.tokenId = 81;
                token.value = str;
                token.tokenString = "TK_READ";
                return token;
            }
            else if(str == "?print"){
                state = 41;
                token.tokenId = 82;
                token.value = str;
                token.tokenString = "TK_PRINT";
                return token;
            }
            else{
                cout<<"Line No: "<<lineNo<<" Syntax Error"<<endl;
                token.tokenId = 999;
                token.tokenString = "TK_ERROR";
                token.value = "Syntax Error";
                return token;
            }
        }
        else if(buffer[offset] == '_'){
            state = 35;
            string str = "";
            string num = "";
            str += buffer[offset];
            offset++;
            int flagalpha = 0;
            int flagnum = 0;
            bool openbrack = false;
            bool closebrack = false;
            int MAX_VARIABLE_LEN = 15;
            int len = 1;
            if(flagalpha == 0){
                while(buffer[offset] >='a' && buffer[offset] <='z'){ // min two alphas must be there to name a var
                    flagalpha = 1;
                    str += buffer[offset];
                    offset++;
                    len++;
                    if(len > MAX_VARIABLE_LEN){
                        // handle error
                        // cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        // token.tokenId = 999;
                        // token.tokenString = "TK_ERROR";
                        // token.value = "Syntax Error";
                        // return token;
                    }
                }
                if(flagalpha == 0){
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Variable should have atleast one alphabet in decleration"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            if(flagalpha == 1 && flagnum == 0){
                
                while(buffer[offset] >='0' && buffer[offset] <='9'){
                    state = 36;
                    flagnum = 1;
                    str += buffer[offset];
                    offset++;
                    len++;  
                    if(len > MAX_VARIABLE_LEN){
                        // cout<<"Error! Line No: "<<lineNo<<" Variable Length is too long to declare"<<endl;
                        // token.tokenId = 999;
                        // token.tokenString = "TK_ERROR";
                        // token.value = "Syntax Error";
                        // return token;
                    }
                }
                if(buffer[offset] == '['){
                    state = 37;
                    openbrack = true;
                    str += buffer[offset];
                    offset++;
                    while(buffer[offset] >= '0' && buffer[offset] <= '9'){
                        num += buffer[offset];
                        str += buffer[offset];
                        offset++;
                    }
                    if(buffer[offset] == ']'){
                        state = 38;
                        closebrack = true;
                        str += buffer[offset];
                        offset++;
                    }
                }
                if(flagnum == 0 || openbrack == false || closebrack == false){
                    cout<<"Error! Line No: "<<lineNo<<" Unrecognized Array Syntax "<<str<<endl;
                    // handle error
                    token.tokenId = 999;
                    token.tokenString = "TK_ERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                else if(len > MAX_VARIABLE_LEN){
                        // handle error
                        cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        token.tokenId = 999;
                        token.tokenString = "TK_ERROR";
                        token.value = "Syntax Error";
                        return token;
                    }
                else{
                    token = createArrayToken(str);
                    return token;
                    // token.tokenId = 77;
                    // token.tokenString = "TK_ARRAY";
                    // token.value = str;
                    // return token;
                }
            }

        }
        else{
        cout<<"Error check syntax"<<endl;
        offset++;
        token.value="error_token";
        token.tokenString="TK_ERROR";
        token.tokenId=404;
        return token;
        }
        
    }
    return token;
}

void printTokenList(vector<char>& bytes){
    int morenice = 420; 
    token ans = getNextLexeme(bytes);
    int halwa = ans.tokenId;
    //also check for error lexeme
    while(halwa != morenice){
        
        if(ans.tokenId!=22){
        cout<<"Token "<<ans.tokenId<<", "<<ans.tokenString<<", string "<<ans.value<<", line number "<<lineNo;
        cout<<endl;
        }
        ans = getNextLexeme(bytes);
        halwa = ans.tokenId;
    }
}
 vector<char> getInputStream(string str_name){
    string filename(str_name);
    vector<char> bytes;
    ifstream input_file(filename);
    char byte;
    while(input_file.get(byte)){
        bytes.push_back(byte);
    }
    return bytes;
}
string removeAllComments(string fileName){
    string filename(fileName);
    vector<char> bytes;
    char byte;
    ifstream input_file(filename);
    string output_file_name="removedComments.txt";
    ofstream opFile(output_file_name);
    while(input_file.get(byte)){
       //handles single line comments (# content)
       if(byte=='#'){
         while(byte!='\n'){
             input_file.get(byte);
         }
        //  input_file.get(byte);
       }
       //handles multiline comments {$ content $}
       if(byte=='$'){
           input_file.get(byte);
           while(byte!='$'){
             input_file.get(byte);
         }
          input_file.get(byte);
       }
        
        opFile<<byte;
    }
    
    return output_file_name;
}
int main(){
    lineNo=1;
    createLexerHashTable();
    token t = addGlobalFun();
    cout<<t.tokenId<<endl;
    cout<<t.tokenString<<endl;
    cout<<t.value<<endl;
    string fileName="test1.txt";//can take as a user input
    string f2=removeAllComments(fileName);//this returns the filename of the new file with no comments
    vector<char> bytes=getInputStream(f2);
    printTokenList(bytes);
    
}