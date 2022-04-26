/*
Group No: 29
Kasina Satwik : 2019A7PS0011H
Rohan Rao Nallani : 2019A7PS0048H
Rohith Kumar Gattu : 2019A7PS0049H
Srikar Sashank Mushnuri : 2019A7PS0160H
*/
#include<bits/stdc++.h>
#include<fstream>
#include<iostream>
#include "newlexer.h"
using namespace std;

// ------------------------------------------------------------------------------------------------- //
symbolTable st;

stack<string> functStack;
map<int,hashTable> ht;
// Finding the hash value of the key word or any string.//
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

// If the string or keyword if already exists in the hash table, it returns it's token id or else returns -1
int lookup(string lexeme){

	int in=hashFunc(lexeme);
	if(ht.find(in) != ht.end() && (ht[in].keyWord == lexeme)){

		return ht[in].tokenId;
	}
	return -1;

}
// function to create hash table to store the tokens of the seen strings.
void createLexerHashTable(){
	const int m=1e2+9;
	
	char keywords[11][15]={"Int","Float","If","Else","For","While",
	"Return","String","Void","Array", "main"}; // key words in the Kidney language. 

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

// function which returns the name of the current function in which the scope is present from the test case
string getCurFun(){
    return CUR_FUN_NAME;
}
// if function scope is changed, this function updates the current function
void updateCurFun(string newName){
    CUR_FUN_NAME = newName;
}
bool checkMain(string funName){
    return funName == "main";
}

// creates and returns the new token to the newly seen function.
token createFuncToken(string funName){
    token ans;
    ans.tokenId = funcToken;
    ans.value = funName;
    ans.tokenString = "TKFUNCID";
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
    ans.tokenString="TKMAIN";
    funct fun;
    fun.token=ans;
    fun.lineNo=lineNo;
    fun.name=str;
    fun.specialId=10001 ;//here we set manually
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
    ans.tokenString = "TKGLOBALFUNC";
    ans.value = fun.name;
    st.funct_list.push_back(fun);
    st.funct_map[fun.name] = fun;
    return ans;
}
// stack is used to make store the sequence of scopes of the function. Once the scope ends, it is popped out of stack.
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
        }
        else{
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
// creates token to the newly seen variable type and stores it in the symbol table.
token createVariableToken(string newVariable){
    token ans;
    // createNewVariable(newVariable);
    ans.tokenId = st.funct_map[getCurFun()].specialId;
    st.funct_map[getCurFun()].specialId++;
    ans.tokenString = "TKVARID";
    
    ans.value = newVariable;

    
    return ans;
}

// create a new variable type from the given string, the name of the variable
variable createNewVariable(string newVariable){
    string curfun = getCurFun();
    funct fun = st.funct_map[curfun];
    map<string,variable> mp = fun.funct_variable_map;
    variable ans;
    if(mp.find(newVariable) == mp.end()){
        // cout<<"hdekdewnbwejobfewjw"<<endl;
        ans.identifier = newVariable;
        ans.scope = getCurFun();
        ans.lineNo = lineNo;
        ans.token = createVariableToken(newVariable);
        
        st.funct_map[curfun].funct_variable_map[newVariable] = ans;
        // cout<<fun.funct_variable_map[newVariable].identifier<<"@@@@@@@"<<endl;
        st.funct_map[curfun].funct_variable_list.push_back(ans);
    }
    else{
        // cout<<"xxxxxxxxxxxxxxxxxxxxxxx"<<endl;
        ans = mp[newVariable];
    }
    //  cout<<fun.funct_variable_map[newVariable].identifier<<"@@@@@@@"<<endl;
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
    ans.tokenString = "TKARRAY";
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
        st.funct_map[curfun].funct_array_map[newArray] = ans;
        st.funct_map[curfun].funct_array_list.push_back(ans);
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
            token.value = "@@";
            token.tokenId = 420;
            token.tokenString = "TKEOF";
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
           token.tokenString="TKSPACE";
           return token;
        }
        if(buffer[offset]==';'){
            offset++;
            state=2;
            token.value=";";
            token.tokenString="TKSEMICOLON";
            token.tokenId=401;
            return token;
        }
        else if(buffer[offset] == '^'){
            offset++;
            state = 42;
            token.value = "^";
            token.tokenId = 49;
            token.tokenString = "TKXOR";
            return token;
        }
        else if(buffer[offset] == '&'){
            offset++;
            if(buffer[offset] == '&'){
                state = 4;
                token.value = "&&";
                token.tokenId = 51; 
                token.tokenString = "TKBITAND"; 
                offset++;
                return token;
            }
            else{
                state = 3;
                token.value = "&";
                token.tokenId = 50; 
                token.tokenString = "TKAND"; 
                return token;
            }

        }
        else if(buffer[offset] == '|'){
            offset++;
            if(buffer[offset] == '|'){
                state = 6;
                token.value = "||";
                token.tokenId = 53;
                token.tokenString = "TKBITOR";
                offset++;
                return token;
            }
            else{
                state = 5;
                token.value = "|";
                token.tokenId = 52;
                token.tokenString = "TKOR";
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
                    token.tokenString = "TKNUM";
                    token.value="+"+str1;
                    return token;
                }
                else{
                    ///// handle error
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TKDECIMAL";
                    token.value="+"+str1+fll;
                    return token;
                }
                else{
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            token.value =str1+fll; 
            return token;
        }
            token.value = "+";
            token.tokenId = 54;
            token.tokenString = "TKADD";
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
                    token.tokenString = "TKNUM";
                    token.value="-"+str1;
                    return token;
                }
                else{
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TKDECIMAL";
                    token.value="-"+str1+fll;
                    return token;
                }
                else{
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
            }
            token.value =str1+fll; 
            return token;
        }
            token.value = "-";
            token.tokenId = 55;
            token.tokenString = "TKSUB";
            return token;
        }
        else if(buffer[offset] == '*'){
            offset++;
            state = 11;
            token.value = "*";
            token.tokenId = 56;
            token.tokenString = "TKMUL";
            return token;
        }
        else if(buffer[offset] == '/'){
            offset++;
            state = 12;
            token.value = "/";
            token.tokenId = 57;
            token.tokenString = "TKDIV";
            return token;
        }
        else if(buffer[offset] == '%'){
            offset++;
            state = 13;
            token.value = "%";
            token.tokenId = 58;
            token.tokenString = "TKMOD";
            return token;
        }
        else if(buffer[offset] == '!'){
            offset++;
            if(buffer[offset] == '='){
                state = 15;
                token.value = "!=";
                token.tokenId = 60; 
                token.tokenString = "TKNOTEQ"; 
                offset++;
                return token;
            }
            else{
                state = 14;
                token.value = "!";
                token.tokenId = 59; 
                token.tokenString = "TKNOT"; 
                return token;
            }
        }
        else if(buffer[offset] == '>'){
            offset++;
            if(buffer[offset] == '='){
                state = 17;
                token.value = ">=";
                token.tokenId = 62; 
                token.tokenString = "TKGE"; 
                offset++;
                return token;
            }
            else{
                state = 16;
                token.value = ">";
                token.tokenId = 61; 
                token.tokenString = "TKGT"; 
                return token;
            }
        }
        else if(buffer[offset] == '<'){
            offset++;
            if(buffer[offset] == '='){ 
                state = 19;
                token.value = "<=";
                token.tokenId = 64; 
                token.tokenString = "TKLE"; 
                offset++;
                return token;
            }
            else{
                state = 18;
                token.value = "<";
                token.tokenId = 63; 
                token.tokenString = "TKLT"; 
                return token;
            }
        }
        else if(buffer[offset] == '='){
            offset++;
            if(buffer[offset] == '='){
                state = 20;
                token.value = "==";
                token.tokenId = 65; 
                token.tokenString = "TKEQUALS"; 
                offset++;
                return token;
            }
            else{
                // error 
                cout<<"line:"<<lineNo<<":no such symbol exists, try changing to : or =="<<endl;
                token.tokenId = 999;
                token.tokenString = "TKERROR";
                token.value = "Syntax Error";
                return token;
            }
        }
        else if(buffer[offset] == ':'){
            offset++;
            state = 21;
            token.value = ":";
            token.tokenId = 66;
            token.tokenString = "TKASSIGN";
            return token;
        }
        else if(buffer[offset] == '{'){
            offset++;
            state = 22;
            token.value = "{";
            token.tokenId = 67;
            token.tokenString = "TKFLOWOPEN";
            return token;
        }
        else if(buffer[offset] == '}'){
            offset++;
            state = 23;
            token.value = "}";
            token.tokenId = 68;
            token.tokenString = "TKFLOWCLOSED";
            return token;
        }
        else if(buffer[offset] == '('){
            offset++;
            state = 24;
            token.value = "(";
            token.tokenId = 69;
            token.tokenString = "TKSMOPEN";
            return token;
        }
        else if(buffer[offset] == ')'){
            offset++;
            state = 25;
            token.value = ")";
            token.tokenId = 70;
            token.tokenString = "TKSMCLOSED";
            return token;
        }
        else if(buffer[offset] == '['){
            offset++;
            state = 26;
            token.value = "[";
            token.tokenId = 71;
            token.tokenString = "TKSQOPEN";
            return token;
        }
        else if(buffer[offset] == ']'){
            offset++;
            state = 27;
            token.value = "]";
            token.tokenId = 72;
            token.tokenString = "TKSQCLOSED";
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
                    token.tokenString = "TKNUM";
                    token.value=str1;
                    return token;
                }
                else{
                    ///// handle error
                    cout<<"Error! Line No:"<<lineNo<<" Integer value overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                
            }
            else{
                if(fll.size() <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TKDECIMAL";
                    token.value=str1+fll;
                    return token;
                }
                else{
                    // handle error
                    cout<<"Error! Line No:"<<lineNo<<" Floating point overflow"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
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
            token.tokenString = "TKALPHAARRAY";
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
                    string s = ht[hashFunc(str)].keyWord;

                    for(int i=0;i<s.size();i++){
                        s[i]=toupper(s[i]);
                    }
                    token.tokenString = "TK" + s;
                    return token;
                }
            }
            // handle errora
            cout<<"no such keyword:"<<str<<"please check syntax"<<endl;
            //******we need to change this token to error token
            token.tokenId = 999;
            token.tokenString = "TKERROR";
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
                    cout<<"Error! Line No:"<<lineNo<<" Variable should have atleast two alphabet in decleration"<<endl;
                    token.tokenId = 999;
                    token.tokenString = "TKERROR";
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
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                else if(lenalpha > MAX_VARIABLE_LEN || lennum > MAX_VARIABLE_LEN || lenalpha + lennum > MAX_VARIABLE_LEN){
                        // handle error
                        cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        token.tokenId = 999;
                        token.tokenString = "TKERROR";
                        token.value = "Syntax Error";
                        return token;
                    }
                else{
                    token = addVariable(str);
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
            token.tokenString = "TKFUNCTEND";
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
                token.tokenString = "TKREAD";
                return token;
            }
            else if(str == "?print"){
                state = 41;
                token.tokenId = 82;
                token.value = str;
                token.tokenString = "TKPRINT";
                return token;
            }
            else{
                cout<<"Line No: "<<lineNo<<" Syntax Error"<<endl;
                token.tokenId = 999;
                token.tokenString = "TKERROR";
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
                    token.tokenString = "TKERROR";
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
                    token.tokenString = "TKERROR";
                    token.value = "Syntax Error";
                    return token;
                }
                else if(len > MAX_VARIABLE_LEN){
                        // handle error
                        cout<<"Error! Line No:"<<lineNo<<" Variable Length is too long to declare"<<endl;
                        token.tokenId = 999;
                        token.tokenString = "TKERROR";
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
        token.tokenString="TKERROR";
        token.tokenId=404;
        return token;
        }
        
    }
    return token;
}
// print all the token list.
void printTokenList(vector<char>& bytes){
    int notfound = 420; 
    token ans = getNextLexeme(bytes);
    int tt = ans.tokenId;
    //also check for error lexeme
    while(tt != notfound){
        
        if(ans.tokenId!=22){
        cout<<"Token "<<ans.tokenId<<", "<<ans.tokenString<<", string "<<ans.value<<", line number "<<lineNo;
        cout<<endl;
        }
        ans = getNextLexeme(bytes);
        tt = ans.tokenId;
    }
}

void printTokenListtoFile(vector<char>& bytes){
    int notfound = 420; 
    freopen("Token_gen.txt","w",stdout);
    token ans = getNextLexeme(bytes);
    int tt = ans.tokenId;
    //also check for error lexeme
    while(tt != notfound){
        
        if(ans.tokenId!=22){
        cout<<ans.tokenString<<" ";
        
        }
        ans = getNextLexeme(bytes);
        tt = ans.tokenId;
    }
    cout<<"$"<<endl;
}

// read the input stream - the testcase txt file.
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
// function to remove all the comments given in the test case.
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
// main function.
int main(){
    lineNo=1;
    string fileName="test2.txt";//can take as a user input
    cout<<"Enter file name:"<<endl;
    cin>>fileName;
    createLexerHashTable();
    token t = addGlobalFun();
    cout<<t.tokenId<<endl;
    cout<<t.tokenString<<endl;
    cout<<t.value<<endl;
   
    string f2=removeAllComments(fileName);//this returns the filename of the new file with no comments
    vector<char> bytes=getInputStream(f2);
    //printTokenList(bytes);
    printTokenListtoFile(bytes);
    
}