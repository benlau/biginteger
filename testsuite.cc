/* Test suite for the library.  First, it ``tests'' that all the constructs it
 * uses compile successfully.  Then, its output to stdout is compared to the
 * expected output automatically extracted from slash-slash comments below. */

#include "BigIntegerLibrary.hh"

#include <string>
#include <iostream>
using namespace std;

int main() {
	
BigUnsigned z(0), one(1), ten(10);
cout << z << ',' << one << ',' << ten << endl; //0,1,10

return 0;
}
