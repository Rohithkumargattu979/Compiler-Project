#include<bits/stdc++.h>
#include "parser.h"
#include "intermediateCodegen.h"
using namespace std;

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
void generateTree(vector<string>& arith1 , vector<string>& finalans){ 
  //vector<string> codegens;
  string codegen;
  stack<string> st;
  for(int i=0; i<arith1.size(); i+=1){
    st.push(arith1[i]);
  }
  //cout<<st.size()<<endl;3
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
    codegen += s1;
    st1.pop();
  }
  finalans.push_back(codegen);
  //cout<<finalans.size()<<endl;
  codegen = "";

}

vector<string> generateHelper(vector<vector<string>>& ariths){
    vector<string> finalans;
    for(int i=0; i<ariths.size(); i++){
      //intcount = 1; ->dont' have to re-initialise
      generateTree(ariths[i],finalans);
    }

    return finalans;
}

int f(string &res){
    int c = 0;
    for(int i = 0 ; i<res.size() ; ++i){
        if(res[i] == '\n')
        c++;
    }
    c++;

    return c;
}

int main(){
    freopen("Token_gen_and_id.txt","r",stdin);
    freopen("ifelsecodegen.txt","w",stdout);
    string token , tokenval;
    int gotoelse = 1 , gotofinal = 1 , lineno = 0;
    while(true){
        cin>>token;
        if(token == "$"){
            break;
        }
        string currass = "" ,currass1="";
        vector<vector<string>> ifarithst,elsearithst; 
        if(token == "TKIF"){
            cin>>tokenval;
            while(token != "TKSMCLOSED"){
                if(token != "TKSMOPEN" && token != "TKIF"){
                    currass += tokenval + " ";
                }
                cin >> token;
                cin >> tokenval;

                //cout<<"error 0"<<endl;

            }
            vector<string> bot;
            while(token != "TKFLOWCLOSED"){
                cin>>token;
                cin>>tokenval;
                if(token != "TKSEMICOLON" && token != "TKFLOWOPEN"){
                    bot.push_back(tokenval);
                }
                else if(token == "TKSEMICOLON"){
                ifarithst.push_back(bot);
                bot.clear();
                } 
            }
            vector<string> res = generateHelper(ifarithst);
            ifarithst.clear();
            for(int i=0 ; i<res.size() ; ++i){
                //cout<<res[i]<<endl;
                gotoelse += f(res[i]);
            }
            //gotoelse++;
            lineno++;
            cout<<"If"<<" "<<currass<<" ";
            cout<<"go to"<<" "<<lineno+gotoelse<<endl;
             for(int i=0 ; i<res.size() ; ++i){
                cout<<res[i]<<endl;
            }
        }
            else if(token == "TKELSE"){
            cin>>tokenval;
            vector<string> bot;
            while(token != "TKFLOWCLOSED"){
                //cin>>tokenval;
                if(token != "TKSEMICOLON" && token != "TKFLOWOPEN" && token != "TKELSE"){
                    bot.push_back(tokenval);
                }
                else if(token == "TKSEMICOLON"){
                elsearithst.push_back(bot);
                bot.clear();
                } 

                cin>>token;
                cin>>tokenval;

                //cout<<"error 1"<<endl;

            }
            vector<string> res = generateHelper(elsearithst);
            elsearithst.clear();
            for(int i=0 ; i<res.size() ; ++i){
                //cout<<res[i]<<endl;
                gotofinal += f(res[i]);
            }
            //gotoelse++;
            // cout<<"Else"<<" "<<currass<<" ";
            // cout<<"go to"<<" "<<++gotofinal<<endl;
            //gotoelse ++;
             for(int i=0 ; i<res.size() ; ++i){
                cout<<res[i]<<endl;
                lineno++;
                //gotoelse += f(res[i]);
            }
            // for(int i=0 ; i<ans.size() ; ++i){
            //     //cout<<ans[i]<<endl;
            // }
            // while(token != "TKFLOWCLOSED"){
            //     cin>>token;
            //     cin>>tokenval;
            //     if(token != "TKSEMICOLON" && token != "TKFLOWOPEN"){
            //         bot.push_back(tokenval);
            //     }
            //     else if(token == "TKSEMICOLON"){
            //     ifarithst.push_back(bot);
            //     // for(int i=0 ; i<bot.size() ; ++i){
            //     //     cout<<bot[i]<<" ";
            //     // }
            //     // cout<<endl;
            //     bot.clear();
            //     } 

            //     //cout<<"error 2"<<endl;
            // }
            //             generateHelper(ifarithst);
            //             ifarithst.clear();


            
        }
    }
    // for(int i=0;i<finalans.size();i++){
    //     cout<<finalans[i]<<endl;
    // }
}


// for(int i=0 ; i<ans.size() ; ++i){
            //     //cout<<ans[i]<<endl;
            // }
            // while(token != "TKFLOWCLOSED"){
            //     cin>>token;
            //     cin>>tokenval;
            //     if(token != "TKSEMICOLON" && token != "TKFLOWOPEN"){
            //         bot.push_back(tokenval);
            //     }
            //     else if(token == "TKSEMICOLON"){
            //     ifarithst.push_back(bot);
            //     // for(int i=0 ; i<bot.size() ; ++i){
            //     //     cout<<bot[i]<<" ";
            //     // }
            //     // cout<<endl;
            //     bot.clear();
            //     } 

            //     //cout<<"error 2"<<endl;
            // }
            //             generateHelper(ifarithst);
            //             ifarithst.clear();


            