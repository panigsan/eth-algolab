#include <iostream>
#include <vector>
#include <deque>

using namespace std;


struct Node{
  int level;
  vector<int> next;
};

void testcase(){
  int n,m,v;
  cin >> n >> m >> v;

  vector<Node> nodes;
  for(int i=0;i<n;i++) nodes.push_back({-1,{}});

  for(int i=0;i<m;i++){
    int a,b;
    cin >> a >> b;

    nodes[a].next.push_back(b);
    nodes[b].next.push_back(a);
  }

  deque<int> q;
  q.push_back(v);
  nodes[v].level=0;
  while(!q.empty()){
    Node& t = nodes[q.front()];
    for(int x : t.next){
      Node& suc = nodes[x];
      if(suc.level == -1){
        suc.level = t.level+1;
        q.push_back(x);
      }
    }
    q.pop_front();
  }

  for(Node& x : nodes) cout << x.level << ' ';
  cout << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
