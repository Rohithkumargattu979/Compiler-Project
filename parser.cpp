#include<bits/stdc++.h>
#include "newlexer.h"
#include<fstream>

using namespace std;

map<pair< int,string >,string > mp;
map <int,string> hm;
vector<string> inp_tokens;

struct Node{
    string str;
    vector<Node> children;
};
void buildTree(){

}
void printTree(Node n ){
    cout<<n.str<<" "<<"hi"<<endl;
    for(int i=0;i<n.children.size();i++){
        printTree(n.children[i]);
    }

}
void printTree_level_order(Node n){
    queue<Node> q;
    q.push(n);
    // cout<<q.front()<<"hlkmnl"<<endl;
    while(q.size() != 0){
        Node curr =  q.front();
        cout<<curr.str<<endl;
        for(int i=0 ; i<curr.children.size();i++){
            q.push(curr.children[i]);
        }
        q.pop();

    }
}
void insert(){
    mp[{0,"a"}]="s3";
    mp[{0,"b"}]="s4";
    mp[{1,"$"}]="ACCEPT";
    mp[{0,"A"}]="2";
    mp[{0,"S"}]="1";
    mp[{2,"a"}]="s3";
    mp[{2,"b"}]="s4";
    mp[{2,"A"}]="5";
    mp[{3,"a"}]="s3";
    mp[{3,"b"}]="s4";
    mp[{3,"A"}]="6";
    mp[{4,"a"}]="r3";
    mp[{4,"b"}]="r3";
    mp[{4,"$"}]="r3";
    mp[{5,"a"}]="r2";
    mp[{5,"b"}]="r2";
    mp[{5,"$"}]="r1";
    mp[{6,"a"}]="r2";
    mp[{6,"b"}]="r2";
    mp[{6,"$"}]="r2";

    hm[0]="SX S";
    hm[1]="S A A";
    hm[2]="A a A";
    hm[3]="A b";
}
int calcNo(string x){
    int ans=0;
    for(int i=1;i<x.size();i++){
        ans*=10;
        ans+=(x[i]-'0');
    }
    return ans;
}
bool checkifSlash(string x){
  for(int i=0;i<x.size();i++){
        if(x[i]=='/')return false;
    }
    return true;
}
void splitSpaces(string s,vector<string>& arr){
    string curr="";
    for(int i=0;i<s.size();i++){
        if(s[i]==' '){
            arr.push_back(curr);
            curr="";
        }else{
            curr+=s[i];
        }
    }
    if(curr!=""){
        arr.push_back(curr);
    }
}
Node parse(){
    //pair<TK_MAIN,0> 0 IS THE STATE
   
   stack<pair<string,int>> st;
   stack<Node> node_st;
   st.push({"",0});
   Node n;
   n.str="";
   node_st.push(n);
   string tk;
   int curr=0;
   Node ans;
   while(curr<inp_tokens.size()){
       tk=inp_tokens[curr];
       if(tk=="@")return ans;
       cout<<tk<<" hello"<<endl;
   if(!checkifSlash(mp[{st.top().second,tk}])){
       //handle error
        cout<<"error 0 bot"<<endl;
   }
   if(mp.find({st.top().second,tk})==mp.end()){
       //report error
        cout<<"error 1"<<endl;
   }else if(mp[{st.top().second,tk}][0]=='s'){
        int n_state=calcNo(mp[{st.top().second,tk}]);//
        cout<< tk<<" " <<n_state<<endl;
      st.push({tk,n_state});
      Node nd;
      nd.str=tk;
      node_st.push(nd);
      curr++;

   }else if(mp[{st.top().second,tk}][0]=='r'){
       int rule_no=calcNo(mp[{st.top().second,tk}]);
       //perform reduce
       vector<string> arr;
       splitSpaces(hm[rule_no],arr);
       
       string left=arr[0];
       Node nd;
       nd.str=left;
        vector<Node> v;
       for(int i=arr.size()-1;i>=1;i--){
            // cout<<arr[i]<<endl;
            if(st.top().first==arr[i]){
                st.pop();
                v.push_back(node_st.top());
                node_st.pop();
            }else{
                //error condition
                 cout<<"error 2"<<endl;
            }
       }
       reverse(v.begin() , v.end());
       for(int i=0 ; i<v.size() ; ++i){
           nd.children.push_back(v[i]);
       }
       node_st.push(nd);
       if(nd.str == "S"){
           ans = nd;
        }
       cout<<"reducing using"<<" "<<hm[rule_no]<<endl;
       if(mp.find({st.top().second,left})==mp.end()){
           //error
           cout<<st.top().second<<" "<<tk<<" "<<"boot"<<endl;
            cout<<"error 3"<<endl;
       }
       if(mp[{st.top().second,left}][0]=='s'||mp[{st.top().second,left}][0]=='r'){
           //error 
           cout<<"error 4"<<endl;
       }
       
       int goto_state=stoi(mp[{st.top().second,left}]);
       cout<<"going to state "<<goto_state<<endl;
       st.push({left,goto_state});

   }else if(mp[{st.top().second,tk}]=="ACCEPT"){
       cout<<"ACCEPTED "<<endl;

       return ans;
   }
   else{
      //error
      cout<<"error 5"<<endl;
      curr++;
   }
   
   }
  
   return ans;

}
int main(){
    
    freopen("Token_gen.txt","r",stdin);
    
    string tk;
    while(true){
        cin>>tk;
        inp_tokens.push_back(tk);
        if(tk=="$"){
            break;
        }
    }
    inp_tokens.push_back("@");
    // freopen("parser_op.txt","w",stdout);
    insert();
    Node res = parse();
    cout<<"The Parse Tree generated for our grammar is : " << endl;
    printTree_level_order(res);

}