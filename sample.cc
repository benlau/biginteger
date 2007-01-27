/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

/*
* This sample file demonstrates the most important features of the Big Integer Library.
*
* To get started quickly with the library, imitate the code in `main' below.
*
* If you want more detail or more speed or can't find a feature here,
* look in the appropriate source file.  This file shows only the more ``user-friendly'' features;
* the other features are messier but worth learning eventually.
*
* GO FORTH and play with many-digit numbers!  (c.f. The TeXbook.)
*/

#include "BigUnsigned.hh"
#include "BigInteger.hh"
#include "BigIntegerUtils.hh"

#include <string>
#include <iostream>

int main() {
	try {
		BigInteger a; // a is 0
		int b = 535;
		
		a = b; // From int to BigUnsigned...
		b = a; // ...and back, no casts required!
		/*
		* If a were too big for an int you'd get a runtime exception.  The Big Integer Library
		* throws C-strings (that is, `const char *'s) when something goes wrong.  It's a good
		* idea to catch them.  Some C++ compilers need a special command-line option to compile
		* code that uses throw/catch.
		*/
		
		BigInteger c(a); // Copy them.
		
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
		* The Big Integer Library provides three kinds of operators:
		*
		* (1) Overloaded ``value'' operators: +, -, *, /, %, unary -.
		* Big-integer code using these operators looks identical to
		* code using the primitive integer types.  The operator takes
		* one or two BigInteger inputs and returns a BigInteger result,
		* which can then be assigned to a BigInteger variable or used
		* in an expression.
		*
		* (2) Overloaded assignment operators: +=, -=, *=, /=, %=,
		*     ++, --, flipSign.
		* Again, these are used on BigIntegers just like on ints.
		* They take one writable BigInteger that both provides an
		* operand and receives a result.  The first five also take
		* a second read-only operand.
		*
		* (3) ``Put-here'' operations: `add', `subtract', etc.
		* Use these if and only if you are concerned about performance.
		* They require fewer BigInteger copy-constructions and assignments
		* than do operators in (1) or (2).  Most take two read-only operands
		* and save the result in the invoked object `*this', whose previous
		* value is irrelevant.  `divideWithRemainder' is an exception.
		* <<< NOTE >>>: Put-here operations do not return a value: they don't need to!!
		*/
		
		BigInteger g(314159), h(265);
		// All five ``value'' operators
		std::cout << (g + h) << '\n' << (g - h) << '\n' << (g * h)
			<< '\n' << (g / h) << '\n' << (g % h) << std::endl;
		
		BigInteger i(5), j(10), k;
		// These two lines do the same thing: k is set to a BigInteger containing 15.
		k = i + j;
		k.add(i, j);
		
		// Let's do some heavy lifting.
		std::cout << "Powers of 3" << std::endl;
		std::cout << "How many do you want?" << std::endl;
		int maxPower;
		std::cin >> maxPower;
		
		BigUnsigned x(1), three(3);
		for (int power = 0; power <= maxPower; power++) {
			std::cout << "3^" << power << " = " << x << std::endl;
			x *= three; // A BigInteger assignment operator
		}
		
		std::cout << "There you go.  Goodbye." << std::endl;
		
	} catch(char const* err){
		std::cout << "Sorry, the library threw an exception:\n"
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
