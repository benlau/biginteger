/*
* Big Integer Library
* Filename: BigIntegerIO.h
* Author: Matt McCutchen
* Version: 2004.1205
*/

#include "BigIntegerIO.h"

std::ostream& operator<<(std::ostream &os, BigUnsigned x) {
	if (x.getLength() == 0)
		os << '0';
	else {
		BigUnsigned ten(10);
		
		/*
		* This buffer is filled with the decimal digits of x.
		* sizeof(BigUnsigned::Blk) * x.getLength() is an upper bound on the
		* number of bytes in x, and a byte takes at most 3 decimal
		* digits, so this is a convenient upper bound.
		*/
		char *buf = new char[sizeof(BigUnsigned::Blk) * x.getLength() * 3];
		//std::cout << "bufferlength" << sizeof(BigUnsigned::Blk) * x.getLength() * 3<< std::endl;
		int bufPos = 0; // first unfilled position
		
		// While x is not zero...
		while (!x.isZero()) {
			//std::cout << "bufPos=" << bufPos << std::endl;
			// Get next digit
			buf[bufPos] = char(int(x % ten) + '0');
			// Remove it from x
			x /= ten;
			// Next buffer slot
			bufPos++;
		}
		
		// Now print the digits to os.
		while (bufPos > 0) {
			bufPos--;
			os << buf[bufPos];
		}
		
		// Free the buffer and return the stream (as customary)
		delete buf;
		return os;
	}
}

std::ostream& operator<<(std::ostream &os, BigInteger x) {
	switch (x.getSign()) {
	case BigInteger::positive:
		os << BigUnsigned(x);
		break;
	case BigInteger::zero:
		os << '0';
		break;
	case BigInteger::negative:
		os << '-' << BigUnsigned(x);
		break;
	}
	return os;
}
