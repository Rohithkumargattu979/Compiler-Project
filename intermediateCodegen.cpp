#include<bits/stdc++.h>
#include "parser.h"
#include "intermediateCodegen.h"
using namespace std;
void insertSDD(){
  intmp[0].first = "EXPR EXPR OPERATOR EXPR";
  intmp[0].second = "";
  intmp[1].first = "EXPR TKNUM";
  intmp[1].second = "";
  intmp[2].first = "EXPR TKDECIMAL";
  intmp[2].second = "";
  intmp[3].first = "OPERATOR TKADD";
  intmp[3].second = "";
  intmp[4].first = "OPERATOR TKSUB";
  intmp[4].second = "";
  intmp[5].first = "OPERATOR TKMUL";
  intmp[5].second = "";
  intmp[6].first = "OPERATOR TKDIV";
  intmp[6].second = "";
  intmp[7].first = "OPERATOR TKASSIGN";
  intmp[7].second = "";
}
void insertOPValues(){
  valmap["TKADD"] = "+";
  valmap["TKMUL"] = "*";
  valmap["TKSUB"] = "-";
  valmap["TKDIV"] = "/";
  valmap["TKASSIGN"] = ":";
}
pair<string,string> newTemp(){
  string ans = "";
  ans += "t";
  ans += to_string(intcount);
  intcount++;
  string ans2 = "";
  pair<string,string> p;
  p.first = ans;
  p.second = ans2;
  return p;
}
pair<string,string> calcValue(string s1, string op, string s2){
  pair<string, string> p;
  p = newTemp();
  p.second = s1 + op + s2;
  return p;
}
void generateTree(vector<string>& arith1){
  stack<string> st;
  for(int i=1; i<arith1.size(); i+=2){
    st.push(arith1[i]);
  }
  //cout<<st.size()<<endl;
  while(st.size() > 3){
    string s2 = st.top();
    st.pop();
    string op = st.top();
    st.pop();
    string s1 = st.top();
    st.pop();
    pair<string,string> p;
    p = calcValue(s1,op,s2);
    st.push(p.first);
    codegen += p.first + "=" + p.second + "\n";
  }
  stack<string> st1;
  while(!st.empty()){
    st1.push(st.top());
    st.pop();
  }
  while(!st1.empty()){
    string s1 = st1.top();
    if(s1 == ":"){
      s1 = "=";
    }
    st1.pop();
    codegen += s1;
  }
  codegens.push_back(codegen);
  codegen = "";

}
void generateHelper(){
    for(int i=0; i<ariths.size(); i++){
      //intcount = 1; ->dont' have to re-initialise
      generateTree(ariths[i]);
    }
}
int main(){
  freopen("Token_gen_and_id.txt","r",stdin);
  string token;
  stack<string> bt;
  bool flag = false;
  while(true){
    cin>>token;
    //cout<<token<<endl;
    bt.push(token);
    if(token == "$"){
      break;
    }
    if(token == "TKSEMICOLON" and flag){
      flag = false;
      ariths.push_back(arith);
      arith.clear();
    }
    if(token == "TKASSIGN"){
      bt.pop();
      flag = true;
      string val1 = bt.top();
      bt.pop();
      string val2 = bt.top();
      while(!bt.empty()){
        bt.pop();
      }
      arith.push_back(val2);
      arith.push_back(val1);
    }
    if(flag){
      arith.push_back(token);
    }
  }
  generateHelper();
  freopen("codegen.txt","w",stdout);
  //cout<<"hi";
  cout<<ariths.size()<<endl;
  for(int i=0; i<codegens.size(); i++){
    cout<<codegens[i]<<endl<<endl;
  }
  cout<<endl;

}