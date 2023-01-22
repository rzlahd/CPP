#include <iostream>

main() {
  int result = 0, value  = 1;
  // keep executing the while as long as val is less than or equal to 10
  while (val <= 10)  {
    result += value;   // assigns sum + val to sum
    ++value;        // add 1 to val
  }
  std::cout<< "Sum of 1 to 10 inclusive is "<<result<<std::endl;
}
