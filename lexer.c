#include<stdio.h>
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
                token.value = '&';
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
                token.value = '|';
                token.tokenId = 52;
                token.tokenString = "TK_OR";
            }
        }
        else if(buffer[offset] == '+'){
            offset++;
            state = 104;
            token.value = '+';
            token.tokenId = 54;
            token.tokenString = "TK_ADD";
        }
        else if(buffer[offset] == '-'){
            offset++;
            state = 105;
            token.value = '-';
            token.tokenId = 55;
            token.tokenString = "TK_MINUS";
        }
        else if(buffer[offset] == '*'){
            offset++;
            state = 106;
            token.value = '+';
            token.tokenId = 56;
            token.tokenString = "TK_MUL";
        }
        else if(buffer[offset] == '/'){
            offset++;
            state = 107;
            token.value = '/';
            token.tokenId = 57;
            token.tokenString = "TK_DIV";
        }
        else if(buffer[offset] == '%'){
            offset++;
            state = 108;
            token.value = '+';
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
                token.value = '!';
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
                token.value = '>';
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
                token.value = '<';
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
            token.value = ':';
            token.tokenId = 66;
            token.tokenString = "TK_ASSIGN";
        }
        else if(buffer[offset] == '{'){
            offset++;
            state = 117;
            token.value = '{';
            token.tokenId = 67;
            token.tokenString = "TK_FLOPEN";
        }
        else if(buffer[offset] == '}'){
            offset++;
            state = 118;
            token.value = '}';
            token.tokenId = 68;
            token.tokenString = "TK_FLCLOSE";
        }
        else if(buffer[offset] == '('){
            offset++;
            state = 119;
            token.value = '(';
            token.tokenId = 69;
            token.tokenString = "TK_SMOPEN";
        }
        else if(buffer[offset] == ')'){
            offset++;
            state = 120;
            token.value = ')';
            token.tokenId = 70;
            token.tokenString = "TK_SMCLOSE";
        }
        else if(buffer[offset] == '['){
            offset++;
            state = 121;
            token.value = '[';
            token.tokenId = 71;
            token.tokenString = "TK_SQOPEN";
        }
        else if(buffer[offset] == ']'){
            offset++;
            state = 122;
            token.value = ']';
            token.tokenId = 72;
            token.tokenString = "TK_SQCLOSE";
        }
        
    }

}
int main(){
    
}