#include <iostream>
#include <vector>
#include <set>

using namespace std;

void testcase(){
  int n,m;
  long k;
  cin >> n >> m >> k;
  vector<long> h(n);
  vector<bool> starting(n,true);
  vector<bool> valid(n,false);
  vector<int> parent(n,0);
  for(int i=0;i<n;++i) cin >> h[i];
  
  for(int i=0;i<n-1;++i){
    int u,v;
    cin >> u >> v;
    starting[u] = false;
    parent[v] = u;
  }

  vector<bool> visited(n,false);
  for(int i=0;i<n;++i){
    if(!starting[i]) continue;
   
    multiset<long> window;
    int current = i;
    int l=1;
    int tail = current;
    window.insert(h[i]);
    while(true){
      // if the tail has been visited, get out, no more business here
      if(visited[tail]) break;
      long mini = *window.begin();
      long maxi = *window.rbegin();
      // leg is not safe, remove tail
      if(maxi-mini>k){
        window.erase(window.find(h[tail]));
        l--;
        visited[tail] = true;
        tail = parent[tail];
      }
      // leg is short, add head
      else if(l<m){
        if(current != parent[current]){
          ++l;
          current = parent[current];
          window.insert(h[current]);
        } else { // head is at 0, get out
          visited[tail] = true;
          break;
        }
      // leg is exactly the required length, save it and remove the tail
      } else if(l==m){
        valid[current] = true;
        window.erase(window.find(h[tail]));
        l--;
        visited[tail] = true;
        tail = parent[tail];
      }
    }
  }

  bool one=false;
  for(int i=0;i<n;++i){
    if(valid[i]){
      cout << i << " ";
      one = true;
    } 
  }
  if(!one) cout << "Abort mission";
  cout << endl;

}


int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
