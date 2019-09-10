#include <iostream>
#include <vector>

using namespace std;


void testcase(){
  int n; cin >> n;

  vector<int> bits;

  for(int i=0;i<n;++i){
    int b; cin >> b;
    bits.push_back(b);
  }

  vector<int> sum;
  sum.push_back(bits[0]);
  for(int i=1;i<n;++i){
    sum.push_back(sum[i-1] + bits[i]);
  }
  int evens = 0;
  int odds = 0;

  for(int i=0;i<n;++i){
    if(sum[i]%2==0) ++evens;
    else ++odds;
  }

  int even_pairs = 0;
  even_pairs += evens*(evens-1)/2;
  even_pairs += odds*(odds-1)/2;
  even_pairs += evens;

  cout << even_pairs << endl;

}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;++i){
    testcase();
  }
}

