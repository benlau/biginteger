/*
* Matt McCutchen's Big Integer Library
* http://hashproduct.metaesthetics.net/bigint/
*/

/*
* This sample program demonstrates the most important features of the Big Integer Library.
* To get started quickly, read the code and explanations below.  Then try the program out.
*
* If you want more detail or more speed or can't find a feature here, look in the
* appropriate source file.  This file shows only the more ``user-friendly'' features;
* the other features are messier but worth learning eventually.
*
* GO FORTH and play with many-digit numbers!  (c.f. The TeXbook.)
*/

// Standard libraries
#include <string>
#include <iostream>

// For the BigInteger class itself.
#include "BigInteger.hh"

// For the 4 routines `easy BI/BU <=> string' and `iostream' integration.
#include "BigIntegerUtils.hh"

int main() {
	try {
		std::cout << "=====\nBig Integer Library Demonstration" << std::endl;
		
		BigInteger a; // a is 0
		int b = 535;
		
		a = b; // From int to BigInteger...
		b = a; // ...and back, no casts required!
		/*
		* If a were too big for an int you'd get a runtime exception.  The Big Integer Library
		* throws C-strings (that is, `const char *'s) when something goes wrong.  It's a good
		* idea to catch them; the `try/catch' construct wrapping all this code is an example
		* of how to do this.  Some C++ compilers need a special command-line option to compile
		* code that uses exceptions.
		*/
		
		BigInteger c(a); // Copy a BigInteger.
		
		BigInteger d(-314159265); // c is -314159265.  The `int' literal is converted to a BigInteger.
		
		// Ahem: that's too big to be an `int' literal (or even a `long' literal)!
		// Disillusion yourself now -- this won't compile.
		//BigInteger e(3141592653589793238462643383279);
		
		std::string s("3141592653589793238462643383279");
		BigInteger f = easyStringToBI(s);
		// Ah.  The string is converted to a BigInteger, and strings can be as long as you want.
		
		std::string s2 = easyBItoString(f); // You can convert the other way too.
		
		std::cout << f << std::endl; // f is stringified and send to std::cout.
		
		/*
		* Let's do some math!
		*
		* The Big Integer Library provides lots of overloaded operators
		* and corresponding assignment operators.  So you can do `a + b'
		* with big integers just as with normal integers.  The named
		* methods `add', `divideWithRemainder', etc. are more advanced
		* ``put-here operations''; see `BigUnsigned.hh' for details.
		*/
		BigInteger g(314159), h(265);
		// All five ``return-by-value'' operators.
		std::cout << (g + h) << '\n' << (g - h) << '\n' << (g * h)
			<< '\n' << (g / h) << '\n' << (g % h) << std::endl;
		
		std::cout << "=====\nTest code" << std::endl;
		
		/*
		* If you want to experiment with the library,
		* put your own test code here.
		*/
		
		/*
		* (End of test code)
		*/
		
		// Let's do some heavy lifting.
		std::cout << "=====\nPowers of 3" << std::endl;
		std::cout << "How many do you want?" << std::endl;
		int maxPower;
		std::cin >> maxPower;
		
		BigUnsigned x(1), three(3);
		for (int power = 0; power <= maxPower; power++) {
			std::cout << "3^" << power << " = " << x << std::endl;
			x *= three; // A BigInteger assignment operator
		}
		
		std::cout << "There you go.  Goodbye.\n=====" << std::endl;
		
	} catch(char const* err) {
		std::cout << "=====\nSorry, the library threw an exception:\n"
			<< err << std::endl;
	}
	
	return 0;
}

/*
* Here is the output of a sample run of this sample program:

3141592653589793238462643383279
314424
313894
83252135
1185
134
Powers of 3
How many do you want?
2
3^0 = 1
3^1 = 3
3^2 = 9
There you go.  Goodbye.

*/
