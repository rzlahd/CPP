#include <iostream>
#include <thread>
using namespace std;

void ThreadFunctions(int argm) {
  argm+1;
  cout<<argm<<endl;
}

main() {
  int i;
  thread thread1(ThreadFunctions, i);
  return 0;
}
