#include <iostream>
#include <vector>

using namespace std;

void testcase(){
  int n; cin >> n;

  vector<long> heights;
  for(int i=0;i<n;++i){
    long h; cin >> h;
    heights.push_back(h);
  }

  long i = 0;
  for(long j=1; j<n;++j){
    // if falls AND reaches a further domino
    if(j-i<heights[i] && j+heights[j] > i+heights[i]){
      i = j;
    }
  }

  long falling = i + heights[i];
  if(falling>n) falling = n;

  cout << falling << endl;

}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;++i){
    testcase();
  }
}
