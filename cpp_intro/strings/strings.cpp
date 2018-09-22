#include <iostream>

using namespace std;

void reverse(string& x){
  int l=0;
  int r=x.length()-1;
  
  while(l<r){
    char t = x[r];
    x[r] = x[l];
    x[l] = t;
    l++;
    r--;
  }
}

void testcase(){
  string a; cin >> a;
  string b; cin >> b;

  cout << a.length() << ' ' << b.length() << endl;
  cout << a << b << endl;

  // two different ways of reversing
  reverse(a);
  reverse(b.begin(),b.end());
  char t = b[0];
  b[0] = a[0];
  a[0] = t;
  cout << a << ' '  << b << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++){
    testcase();
  }
}
