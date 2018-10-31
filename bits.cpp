#include <iostream>
#include <bitset>

using namespace std;

int main(){
  int x = (1<<3) | (1<<4) | (1<<7);
  cout << bitset<8>(x) << endl;

  cout << __builtin_popcount(x) << endl;

  int b=0;
  do {
    cout << bitset<8>(b) << endl;
  } while ((b=(b-x)&x));

}
