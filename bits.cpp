#include <iostream>
#include <bitset>
#include <queue>

using namespace std;

int main(){
  priority_queue<int> Q;
  Q.push(1);
  Q.push(2);
  Q.push(100);
  while(!Q.empty()){
    cout << Q.top() << endl;
    Q.pop();
  }
  int x = (1<<3) | (1<<4) | (1<<7);
  cout << bitset<8>(x) << endl;

  cout << __builtin_popcount(x) << endl;

  // trick to iterate over all submasks of the masks x
  int b=0;
  do {
    cout << bitset<8>(b) << endl;
  } while ((b=(b-x)&x));

}
