#include<stdio.h>
#include"lexer.h"
#include"lexerDef.h"
#include<stdlib.h>
#include<string.h>
#define true 1
FILE *getStream(FILE* fp,char* buffer,int bsize){
	int count;
	if(!feof(fp))
		count=fread(buffer,1,bsize,fp);

	if(count<bsize)
	buffer[count] = '\0';

	return fp;

}
hashTable** ht;
long long hashFunc(char* str){
    long long p=31;
    long long mod=1e9+7;
    long long ans=0;
    for(int i=0;i<strlen(str);i++){
        ans+=((str[i]*p)%mod);
        p*=p;
        p%=mod;

    }
    return mod;
}
int lookup(char* lexeme){

	int in=hashFunc(lexeme);
	if(ht[in]!=NULL && !strcmp(ht[in]->keyWord,lexeme)){

		return ht[in]->tokenId;
	}
	return -1;

}
void createLexerHashTable(){
	const int m=1e2+9;

	ht=(hashTable**)malloc(m*sizeof(ht));
	
	char keywords[11][15]={"Int","Float","If","Else","For","While",
	"Return","String","Void","Array", "Main"}; //add few keywords if needed

	int tokenids[11]={1,2,3,4,5,6,7,8,9,10,11};

	for(int j=0;j<11;j++){
		ht[hashFunc(keywords[j])]=(hashTable*)malloc(sizeof(ht));
		ht[hashFunc(keywords[j])]->keyWord=(char*)malloc(15*sizeof(char));
		strcpy(ht[hashFunc(keywords[j])]->keyWord,keywords[j]);
		ht[hashFunc(keywords[j])]->tokenId=tokenids[j];
	}

}
offset = 0;
token getNextToken(FILE *fp, char* buffer,int bufsize){
    if(ftell(fp)==0){
		createLexerHashTable();
	}
	state = 1;
	char* lexeme = (char*) malloc(bufsize*sizeof(char));
	memset(lexeme, 0, bufsize);
	int i = 0;
	token token;
	token.value = (char*) malloc(bufsize*sizeof(char));
	token.tokenId=0;
    token.tokenString = "";

    while(true){
        //printf("hello");
        if(buffer[offset] == '&'){
            offset++;
            if(buffer[offset] == '&'){
                state = 102;
                token.value = "&&";
                token.tokenId = 51; 
                token.tokenString = "TK_LOGAND"; 
                offset++;
            }
            else{
                state = 2;
                token.value = "&";
                token.tokenId = 50; 
                token.tokenString = "TK_AND"; 
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
            }
            else{
                state = 3;
                token.value = "|";
                token.tokenId = 52;
                token.tokenString = "TK_OR";
            }
        }
        else if(buffer[offset] == '+'){
            offset++;
            state = 104;
            token.value = "+";
            token.tokenId = 54;
            token.tokenString = "TK_ADD";
        }
        else if(buffer[offset] == '-'){
            offset++;
            state = 105;
            token.value = "-";
            token.tokenId = 55;
            token.tokenString = "TK_MINUS";
        }
        else if(buffer[offset] == '*'){
            offset++;
            state = 106;
            token.value = "*";
            token.tokenId = 56;
            token.tokenString = "TK_MUL";
        }
        else if(buffer[offset] == '/'){
            offset++;
            state = 107;
            token.value = "/";
            token.tokenId = 57;
            token.tokenString = "TK_DIV";
        }
        else if(buffer[offset] == '%'){
            offset++;
            state = 108;
            token.value = "o/o";
            token.tokenId = 58;
            token.tokenString = "TK_MOD";
        }
        else if(buffer[offset] == '!'){
            offset++;
            if(buffer[offset] == '='){
                state = 110;
                token.value = "!=";
                token.tokenId = 60; 
                token.tokenString = "TK_NOTEQ"; 
                offset++;
            }
            else{
                state = 109;
                token.value = "!";
                token.tokenId = 59; 
                token.tokenString = "TK_NOT"; 
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
            }
            else{
                state = 111;
                token.value = ">";
                token.tokenId = 61; 
                token.tokenString = "TK_GET"; 
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
            }
            else{
                state = 113;
                token.value = "<";
                token.tokenId = 63; 
                token.tokenString = "TK_LET"; 
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
        }
        else if(buffer[offset] == '{'){
            offset++;
            state = 117;
            token.value = "{";
            token.tokenId = 67;
            token.tokenString = "TK_FLOPEN";
        }
        else if(buffer[offset] == '}'){
            offset++;
            state = 118;
            token.value = "}";
            token.tokenId = 68;
            token.tokenString = "TK_FLCLOSE";
        }
        else if(buffer[offset] == '('){
            offset++;
            state = 119;
            token.value = "(";
            token.tokenId = 69;
            token.tokenString = "TK_SMOPEN";
        }
        else if(buffer[offset] == ')'){
            offset++;
            state = 120;
            token.value = ")";
            token.tokenId = 70;
            token.tokenString = "TK_SMCLOSE";
        }
        else if(buffer[offset] == '['){
            offset++;
            state = 121;
            token.value = "[";
            token.tokenId = 71;
            token.tokenString = "TK_SQOPEN";
        }
        else if(buffer[offset] == ']'){
            offset++;
            state = 122;
            token.value = "]";
            token.tokenId = 72;
            token.tokenString = "TK_SQCLOSE";
        }
        else if(buffer[offset] >= '0' && buffer[offset] <= '9'){
            char* str1;
            char* str2 = buffer[offset];
            strcat(str1, str2);
            offset++;
            state = 123;
            while(buffer[offset] >= '0' && buffer[offset] <='9'){
                str2 = buffer[offset];
                strcat(str1, str2);
                state = 123;
                offset++;
            }
            int flag = 0;
            char* fll = "";
            if(buffer[offset] == '.'){
                flag = 1;
                state = 124;
                char* str3 = buffer[offset];
                offset++;
                strcat(fll,str3);
                while(buffer[offset] >= '0' && buffer[offset] <='9'){
                    str3 = buffer[offset];
                    strcat(fll,str3);
                    state = 124;
                    offset++;
                }
            }
            int MAX_FLOAT_LEN = 10;
            int MAX_INT_LEN = 8;
            if(flag == 0){
                if(strlen(str1) <= MAX_INT_LEN){ // len before '.'
                    token.tokenId = 73;
                    token.tokenString = "TK_INT";
                }
                else{
                    ///// handle error
                }
                
            }
            else{
                if(strlen(fll) <=MAX_FLOAT_LEN){
                    token.tokenId = 74;
                    token.tokenString = "TK_FLOAT";
                }
                else{
                    // handle error
                }
            }
            token.value = strcat(str1,fll); 
        }
        else if(buffer[offset]=='"'){
            char* str1;
            char* str2 = buffer[offset];
            strcat(str1, str2);
            offset++;
            while(buffer[offset]!='"'){
                str2=buffer[offset];
                strcat(str1, str2);
                offset++;
            }
            strcat(str1,str2);
            offset++;
            token.value = str1;
            token.tokenId = 75;
            token.tokenString = "TK_STRING";
        }
        else if(buffer[offset] >= 'A' && buffer[offset] <= 'Z'){
            char* str;
            strcat(str,buffer[offset]);
            offset++;
            while(buffer[offset] >= 'a' && buffer[offset] <= 'z'){
                strcat(str,buffer[offset]);
                offset++;
                if(lookup(str) != -1){
                    token.tokenId = ht[hashFunc(str)]->tokenId;
                    token.value = str;
                    token.tokenString = strcat("TK_",ht[hashFunc(str)]->keyWord);
                    return token;
                }
            }
            // handle error
        }
        else if(buffer[offset] >= 'a' && buffer[offset] <='z'){
            char* str;
            strcat(str,buffer[offset]);
            int flagalpha = 0;
            int flagnum = 0;
            offset++;
            int MAX_VARIABLE_LEN = 15;
            int len = 1;
            if(flagalpha == 0){
                while(buffer[offset] >='a' && buffer[offset] <='z'){
                    flagalpha = 1;
                    strcat(str,(char*)buffer[offset]);
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
                    strcat(str,(char*)buffer[offset]);
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
    FILE* fp;
    fp = fopen("input.txt","r");
    int bufsize = 20;
    char buffer[] = "";
    char c = fgetc(fp);
    while(c != EOF){
        printf("%c\n", c);
        //printf("%s\n", buffer);
        //strcat(buffer,&c);
        //printf("%s\n", buffer);
        c = fgetc(fp);
    }
    //printf("%s", buffer);
    printf("fuck1");
    //token t = getNextToken(fp, buffer, bufsize);
    // printf("fuck3");
    // printf("%d", t.tokenId);
    // printf("%s", t.tokenString);
    // printf("%s", t.value);
}