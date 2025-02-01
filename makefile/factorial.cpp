#include <bits/stdc++.h>

// Definition of factorial function
// is present in function.h file
#include "function.h"
using namespace std;

// Recursive factorial program
int factorial(int n)
{
    if (n == 1)
        return 1;
    return n * factorial(n - 1);
}