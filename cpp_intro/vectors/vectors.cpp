#include <iostream>
#include <vector>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> A;
  for(int i=0;i<n;i++){
    int a; cin >> a;
    A.push_back(a);
  }
  int d; cin >> d;
  A.erase(A.begin()+d);

  int a; cin >> a;
  int b; cin >> b;
  A.erase(A.begin()+a,A.begin()+b+1);
  for(int x : A){
    cout << x << ' ';
  }
  cout << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
