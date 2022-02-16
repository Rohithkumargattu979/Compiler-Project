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
// token that should be generated
typedef struct{
    string keyWord;
    int tokenId;
} hashTable; // hashtable attributes


int state;
int offset = 0;
int lexicalError;
int lineNo;

void createLexerHashTable();
long long hashFunction(string string);
int lookupFunction(string lexeme);

void removeComments(FILE* testfile);

int getLexErrors();
void resetLexErrors();

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
	"Return","String","Void","Array", "Main"}; //add few keywords if needed

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
            state=1;
            token.value=";";
            token.tokenString="TK_SEMICOLON";
            token.tokenId=401;
            return token;
        }
        if(buffer[offset] == '&'){
            offset++;
            if(buffer[offset] == '&'){
                state = 102;
                token.value = "&&";
                token.tokenId = 51; 
                token.tokenString = "TK_LOGAND"; 
                offset++;
                return token;
            }
            else{
                state = 2;
                token.value = "&";
                token.tokenId = 50; 
                token.tokenString = "TK_AND"; 
                return token;
            }

        }
        else if(buffer[offset] == '|'){
            offset++;
            if(buffer[offset] == '|'){
                state = 103;
                token.value = "||";
                token.tokenId = 53;
                token.tokenString = "TK_LOGOR";
                offset++;
                return token;
            }
            else{
                state = 3;
                token.value = "|";
                token.tokenId = 52;
                token.tokenString = "TK_OR";
                return token;
            }
        }
        else if(buffer[offset] == '+'){
            offset++;
            state = 104;
            if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            string str1;
            char str2 = buffer[offset];
            str1 = str1 + str2;
            offset++;
            state = 123;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 123;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 124;
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
            state = 105;
            if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            string str1;
            char str2 = buffer[offset];
            str1 = str1 + str2;
            offset++;
            state = 123;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 123;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 124;
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
                    token.value="-"+str1;
                    return token;
                }
                else{
                    ///// handle error
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
                    // handle error
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
            cout<<"fuckoff"<<endl;
            offset++;
            state = 106;
            token.value = "*";
            token.tokenId = 56;
            token.tokenString = "TK_MUL";
            return token;
        }
        else if(buffer[offset] == '/'){
            offset++;
            state = 107;
            token.value = "/";
            token.tokenId = 57;
            token.tokenString = "TK_DIV";
            return token;
        }
        else if(buffer[offset] == '%'){
            offset++;
            state = 108;
            token.value = "o/o";
            token.tokenId = 58;
            token.tokenString = "TK_MOD";
            return token;
        }
        else if(buffer[offset] == '!'){
            offset++;
            if(buffer[offset] == '='){
                state = 110;
                token.value = "!=";
                token.tokenId = 60; 
                token.tokenString = "TK_NOTEQ"; 
                offset++;
                return token;
            }
            else{
                state = 109;
                token.value = "!";
                token.tokenId = 59; 
                token.tokenString = "TK_NOT"; 
                return token;
            }
        }
        else if(buffer[offset] == '>'){
            offset++;
            if(buffer[offset] == '='){
                state = 112;
                token.value = ">=";
                token.tokenId = 62; 
                token.tokenString = "TK_GEQ"; 
                offset++;
                return token;
            }
            else{
                state = 111;
                token.value = ">";
                token.tokenId = 61; 
                token.tokenString = "TK_GET"; 
                return token;
            }
        }
        else if(buffer[offset] == '<'){
            offset++;
            if(buffer[offset] == '='){ 
                state = 114;
                token.value = "<=";
                token.tokenId = 64; 
                token.tokenString = "TK_LEQ"; 
                offset++;
                return token;
            }
            else{
                state = 113;
                token.value = "<";
                token.tokenId = 63; 
                token.tokenString = "TK_LET"; 
                return token;
            }
        }
        else if(buffer[offset] == '='){
            offset++;
            if(buffer[offset] == '='){
                state = 115;
                token.value = "==";
                token.tokenId = 65; 
                token.tokenString = "TK_EQUALS"; 
                offset++;
                return token;
            }
            else{
                // error 
                cout<<"line:"<<lineNo<<":no such symbol probably try changing to : or =="<<endl;
            }
        }
        else if(buffer[offset] == ':'){
            offset++;
            state = 116;
            token.value = ":";
            token.tokenId = 66;
            token.tokenString = "TK_ASSIGN";
            return token;
        }
        else if(buffer[offset] == '{'){
            offset++;
            state = 117;
            token.value = "{";
            token.tokenId = 67;
            token.tokenString = "TK_FLOPEN";
            return token;
        }
        else if(buffer[offset] == '}'){
            offset++;
            state = 118;
            token.value = "}";
            token.tokenId = 68;
            token.tokenString = "TK_FLCLOSE";
            return token;
        }
        else if(buffer[offset] == '('){
            offset++;
            state = 119;
            token.value = "(";
            token.tokenId = 69;
            token.tokenString = "TK_SMOPEN";
            return token;
        }
        else if(buffer[offset] == ')'){
            offset++;
            state = 120;
            token.value = ")";
            token.tokenId = 70;
            token.tokenString = "TK_SMCLOSE";
            return token;
        }
        else if(buffer[offset] == '['){
            offset++;
            state = 121;
            token.value = "[";
            token.tokenId = 71;
            token.tokenString = "TK_SQOPEN";
            return token;
        }
        else if(buffer[offset] == ']'){
            offset++;
            state = 122;
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
            state = 123;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                str1 += str2;
                state = 123;
                offset++;
            }
            int flag = 0;
            string fll = "";
            if(buffer[offset] == '.'){
               
                flag = 1;
                state = 124;
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
                    token.value=str1;
                    return token;
                }
                else{
                    ///// handle error
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
                }
            }
            token.value =str1+fll; 
            return token;
        }
        else if(buffer[offset]=='"'){
            string str1;
            char str2 = buffer[offset];
            str1 += str2;
            offset++;
            while(buffer[offset]!='"'){
                str2=buffer[offset];
                str1 += str2;
                offset++;
            }
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
            return token;
        }
        else if(buffer[offset] >= 'a' && buffer[offset] <='z'){
            string str;
            str +=  buffer[offset];
            int flagalpha = 0;
            int flagnum = 0;
            offset++;
            int MAX_VARIABLE_LEN = 15;
            int len = 1;
            if(flagalpha == 0){
                while(buffer[offset] >='a' && buffer[offset] <='z'){
                    flagalpha = 1;
                    str += buffer[offset];
                    offset++;
                    len++;
                    if(len > MAX_VARIABLE_LEN){
                        // handle error
                    }
                }
                if(flagalpha == 0){
                    // handle error
                }
            }
            if(flagalpha == 1 && flagnum == 0){
                
                while(buffer[offset] >='0' && buffer[offset] <='9'){
                    flagnum = 1;
                    str += buffer[offset];
                    offset++;
                    len++;  
                    if(len > MAX_VARIABLE_LEN){
                        // handle error
                    }
                }
                if(flagnum == 0){
                    // handle error
                }
            }
            else{
                // error
            }   
        }else{
        cout<<"error check syntax"<<endl;
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
        cout<<ans.tokenId<<" "<<ans.tokenString<<" "<<ans.value<<" "<<lineNo;
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
    string fileName="test1.txt";//can take as a user input
    string f2=removeAllComments(fileName);//this returns the filename of the new file with no comments
    vector<char> bytes=getInputStream(f2);
    printTokenList(bytes);
    
}