/* Test suite for the library.  First, it ``tests'' that all the constructs it
 * uses compile successfully.  Then, its output to stdout is compared to the
 * expected output automatically extracted from slash-slash comments below. */

#include "BigIntegerLibrary.hh"

#include <string>
#include <iostream>
using namespace std;

const BigUnsigned &check(const BigUnsigned &x) {
	unsigned int l = x.getLength();
	if (l != 0 && x.getBlock(l-1) == 0)
		cout << "Unzapped number!" << endl;
	if (l > x.getCapacity())
		cout << "Capacity inconsistent with length!" << endl;
	return x;
}

const BigInteger &check(const BigInteger &x) {
	if (x.getSign() == 0 && !x.getMagnitude().isZero())
		cout << "Sign should not be zero!" << endl;
	if (x.getSign() != 0 && x.getMagnitude().isZero())
		cout << "Sign should be zero!" << endl;
	check(x.getMagnitude());
	return x;
}

#define THROWY(x) try {x; cout << "Expected error not thrown!" << endl; } catch (const char *err) {}

int main() {

try {

BigUnsigned z(0), one(1), ten(10);
cout << z << ',' << one << ',' << ten << endl; //0,1,10

// TODO: Comprehensively test the general and special cases of each function.

// Addition
cout << check(BigUnsigned(0) + 0) << endl; //0
cout << check(BigUnsigned(0) + 1) << endl; //1
cout << check(BigUnsigned(0xFFFFFFFFU) + 1) << endl; //4294967296

// Negative BigUnsigneds
THROWY(BigUnsigned(-1));
THROWY(BigUnsigned(5) - BigUnsigned(6));
cout << check(BigUnsigned(5) - BigUnsigned(5)) << endl; //0

// Multiplication and division
BigUnsigned a = check(BigUnsigned(314159265) * 358979323);
cout << a << ',' << (a / 123) << ',' << (a % 123) << endl;
//112776680263877595,916883579381118,81
THROWY(BigUnsigned(5) / 0);

{
	/* Test that BigInteger division sets the sign to zero.
	 * Bug reported by David Allen. */
	BigInteger num(3), denom(5), quotient;
	num.divideWithRemainder(denom, quotient);
	check(quotient);
	num = 5;
	num.divideWithRemainder(denom, quotient);
	check(num);
}

{
	/* Test that BigInteger subtraction sets the sign properly.
	 * Bug reported by Samuel Larkin. */
	BigInteger zero(0), three(3), ans;
	ans = zero - three;
	cout << check(ans).getSign() << endl; //-1
}

{
	/* Test that BigInteger multiplication shifts bits properly on systems
	 * where long is bigger than int.  (Obviously, this would only catch the
	 * bug when run on such a system.)
	 * Bug reported by Mohand Mezmaz. */
	BigInteger f=4; f*=3;
	cout<<check(f)<<endl; //12
}

{
	/* Test that bitwise XOR allocates the larger length.
	 * Bug reported by Sriram Sankararaman. */
	BigUnsigned a(0), b(3), ans;
	ans = a ^ b;
	cout << ans << endl; //3
}

{
	/* Test that an aliased multiplication works.
	 * Bug reported by Boris Dessy. */
	BigInteger num(5);
	num *= num;
	cout << check(num) << endl; //25
}

} catch (const char *err) {
	cout << "ERROR: " << err << endl;
}

return 0;
}
