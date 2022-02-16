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
    long long mod=1e9+7;
    long long ans=0;
    for(int i=0;i<str.size();i++){
        ans+=((str[i]*p)%mod);
        p*=p;
        p%=mod;

    }
    return mod;
}
int lookup(string lexeme){

	int in=hashFunc(lexeme);
	if(ht.find(in) != ht.end() && !(ht[in].keyWord == lexeme)){

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
        ht[hashFunc(keywords[j])].keyWord = keywords[j];
		ht[hashFunc(keywords[j])].tokenId=tokenids[j];
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
            token.value = "+";
            token.tokenId = 54;
            token.tokenString = "TK_ADD";
            return token;
        }
        else if(buffer[offset] == '-'){
            offset++;
            state = 105;
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
            str1+=str2;
            offset++;
            token.value = str1;
            token.tokenId = 75;
            token.tokenString = "TK_STRING";
            return token;
        }
        else if(buffer[offset] >= 'A' && buffer[offset] <= 'Z'){
            string str;
            str += buffer[offset];
            offset++;
            while(buffer[offset] >= 'a' && buffer[offset] <= 'z'){
                str += buffer[offset];
                offset++;
                if(lookup(str) != -1){
                    token.tokenId = ht[hashFunc(str)].tokenId;
                    token.value = str;
                    token.tokenString = "TK_" + ht[hashFunc(str)].keyWord;
                    return token;
                }
            }
            // handle error
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
        }
    }
    return token;
}


int main(){
    string filename("test1.txt");
    vector<char> bytes;
    ifstream input_file(filename);
    char byte;
    while(input_file.get(byte)){
        bytes.push_back(byte);
    }
    int morenice = 420; 
    token ans = getNextLexeme(bytes);
    int halwa = ans.tokenId;
    while(halwa != morenice){
        ans = getNextLexeme(bytes);
        cout<<ans.tokenId<<" "<<ans.tokenString;
        cout<<endl;
        halwa = ans.tokenId;
    }
    
}