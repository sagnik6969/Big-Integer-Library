/* Example program for bigint class */

#include "./bigint.h"
#include <iostream>
using namespace std;

bigint bigint_fact(long long int n)
{

    bigint i = 1;
    for (int j = 1; j <= n; j++)
        i = i * j;

    return i;
}

int main()
{

    cout << bigint_fact(500);
}