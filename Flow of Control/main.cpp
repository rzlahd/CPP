#include <iostream>

main() {
  int result = 0, value  = 1;
  /* Sytax
  while (condition)
    statement
  */
  // keep executing the while as long as value is less than or equal to 10 because value less then 10
  while (value <= 10)  {
    result += value;   // assigns result + value to result
    ++value;        // add 1 to value
  }
  std::cout<< "Sum of 1 to 10 inclusive is "<<result<<std::endl;
}
