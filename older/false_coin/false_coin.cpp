#include <iostream>
#include <vector>
#include <unordered_set>


using namespace std;

#define trace(x) cerr << #x << " = " << x << endl

typedef vector<pair<unordered_set<int>,unordered_set<int>>> EQ;

bool valid_greater(int i,const EQ& eq, const vector<char> &sign){
    for(int j=0;j<eq.size();++j){
        if(sign[j]=='='){
            if(eq[j].first.count(i) || eq[j].second.count(i)) return false;
        } else if(sign[j]=='>'){
            if(eq[j].second.count(i)) return false;
        } else if(sign[j]=='<'){
            if(eq[j].first.count(i)) return false;
        } 
    }
    return true;
}
bool valid_smaller(int i,const EQ& eq, const vector<char> &sign){
    for(int j=0;j<eq.size();++j){
        if(sign[j]=='='){
            if(eq[j].first.count(i) || eq[j].second.count(i)) return false;
        } else if(sign[j]=='>'){
            if(eq[j].first.count(i)) return false;
        } else if(sign[j]=='<'){
            if(eq[j].second.count(i)) return false;
        } 
    }
    return true;
}

void testcase(){
    int n,k;
    cin >> n >> k;

    vector<char> sign(k);
    EQ eq(k,{unordered_set<int>(),unordered_set<int>()});
    
    for(int i=0;i<k;++i){
        int p;
        cin >> p;
        for(int j=0;j<p;++j) { 
            int pi; 
            cin >> pi; 
            eq[i].first.insert(pi);
        }
        for(int j=0;j<p;++j) { 
            int pi; 
            cin >> pi; 
            eq[i].second.insert(pi);
        }
        char c;
        cin >> c;
        sign[i] = c;
    }

    vector<bool> suspect(n+1,true);
    vector<bool> occurs_left(n+1,false), occurs_right(n+1,false);
    for(int i=0;i<k;++i){
        if(sign[i]=='='){
            // those are for sure safe
            for(int x : eq[i].first)  suspect[x] = false;
            for(int x : eq[i].second) suspect[x] = false;
        } else {
            vector<bool> occurs(n+1,false);
            for(int x : eq[i].first)  occurs[x] = true;
            for(int x : eq[i].second) occurs[x] = true;
            if(sign[i]=='>'){
                for(int x : eq[i].first)  occurs_left[x] = true;
                for(int x : eq[i].second) occurs_right[x] = true;
            } else {
                for(int x : eq[i].first)  occurs_right[x] = true;
                for(int x : eq[i].second) occurs_left[x] = true;
            }
            // numbers not occured in a inequality are also not suspect
            for(int x=1;x<=n;++x) if(!occurs[x]) suspect[x]=false;
        } 
    }
    for(int x=1;x<=n;++x){
        if(occurs_left[x] && occurs_right[x]) suspect[x]=false;    
    }

    vector<int> sols;
    for(int x=1;x<=n;++x) if(suspect[x]) sols.push_back(x);
    if(sols.size()==1) cout << sols[0] << endl;
    else               cout << 0 << endl;


}

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    while(T--) testcase();
}

