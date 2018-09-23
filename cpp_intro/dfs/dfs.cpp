#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
  vector<int> next;
  int time_in;
  int time_out;
};

void testcase(){
  int n; cin >> n;
  int m; cin >> m;
  int v; cin >> v;

  vector<Node> nodes;
  for(int i=0;i<n;i++){
    nodes.push_back({{},-1,-1});
  }

  for(int i=0;i<m;i++){
    int a; cin >> a;
    int b; cin >> b;
    
    nodes[a].next.push_back(b);
    nodes[b].next.push_back(a);
  }
  
  stack<int> q;
  q.push(v);
  int tim = 0;
  while(!q.empty()){
    Node& t = nodes[q.top()];
    if(t.time_in == -1){
      t.time_in = tim;
      tim++;

      vector<int>& next = t.next;
      sort(next.begin(),next.end(),greater<int>());
      for(int x : next){
        if(nodes[x].time_in == -1){ 
          q.push(x); 
        }
      }
    }
    else {
      if(t.time_out == -1) {
        t.time_out = tim;
        tim++;
      }
      q.pop();
    }
  }

  for(int i=0;i<n;i++) cout << nodes[i].time_in << ' ';
  cout << endl;
  for(int i=0;i<n;i++) cout << nodes[i].time_out << ' ';
  cout << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
