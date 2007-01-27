/*
* Matt McCutchen's Big Integer Library
* See: http://mysite.verizon.net/mccutchen/bigint/
*/

#include "BigInteger.h"

// MANAGEMENT

// Assignment operator
void BigInteger::operator =(const BigInteger &x) {
	// Calls like a = a have no effect
	if (this == &x)
		return;
	// Copy sign
	sign = x.sign;
	// Copy the rest
	BigUnsigned::operator =(x);
}

// Constructor from an array of blocks and a sign
BigInteger::BigInteger(const Blk *b, BlkNum l, Sign s) : BigUnsigned(b, l) {
	switch (s) {
		case zero:
		case positive:
		case negative:
		sign = (len == 0) ? zero : s;
		break;
		default:
		throw "BigInteger::BigInteger(Blk *, BlkNum, Sign): Invalid sign";
	}
}

// Constructor from a BigUnsigned and a sign
BigInteger::BigInteger(const BigUnsigned &x, Sign s) : BigUnsigned(x) {
	switch (s) {
		case zero:
		case positive:
		case negative:
		sign = (len == 0) ? zero : s;
		break;
		default:
		throw "BigInteger::BigInteger(Blk *, BlkNum, Sign): Invalid sign";
	}
}

/*
* The steps for construction of a BigInteger
* from an integral value x are as follows:
* 1. If x is zero, create an empty BigInteger and stop.
* 2. Allocate a one-block number array.
* 3. If x is positive (or of an unsigned type), set the
*    sign of the BigInteger to positive.
* 4. If x is of a signed type and is negative, set the
*    sign of the BigInteger to negative.
* 5. If x is of a signed type, convert x (or -x if x < 0)
*    to the unsigned type of the same length.
* 6. Expand x (or the result of step 5) to a Blk,
*    and store it in the number array.
*/

BigInteger::BigInteger(unsigned long x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	}
}

BigInteger::BigInteger(long x) {
	if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	} else if (x < 0) {
		cap = 1;
		blk = new Blk[1];
		sign = negative;
		len = 1;
		*blk = Blk(-x);
	} else {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	}
}

BigInteger::BigInteger(unsigned int x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	}
}

BigInteger::BigInteger(int x) {
	if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	} else if (x < 0) {
		cap = 1;
		blk = new Blk[1];
		sign = negative;
		len = 1;
		*blk = Blk(-x);
	} else {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	}
}

BigInteger::BigInteger(unsigned short x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	}
}

BigInteger::BigInteger(short x) {
	if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		sign = positive;
		len = 1;
		*blk = Blk(x);
	} else if (x < 0) {
		cap = 1;
		blk = new Blk[1];
		sign = negative;
		len = 1;
		*blk = Blk(-x);
	} else {
		cap = 0;
		blk = new Blk[0];
		sign = zero;
		len = 0;
	}
}

// CONVERTERS
/*
* The steps for conversion of a BigInteger to an
* integral type are as follows:
* 1. If the BigInteger is zero, return zero.
* 2. If the BigInteger is positive:
*    3. If it is more than one block long or its lowest
*       block has bits set out of the range of the target
*       type, throw an exception.
*    4. Otherwise, convert the lowest block to the
*       target type and return it.
* 5. If the BigInteger is negative:
*    6. If the target type is unsigned, throw an exception.
*    7. If it is more than one block long or its lowest
*       block has bits set out of the range of the target
*       type, throw an exception.
*    8. Otherwise, convert the lowest block to the
*       target type, negate it, and return it.
*/

namespace {
	// These masks are used to test whether a Blk has bits
	// set out of the range of a smaller integral type.  Note
	// that this range is not considered to include the sign bit.
	const BigUnsigned::Blk  lMask = ~0 >> 1;
	const BigUnsigned::Blk uiMask = (unsigned int)(~0);
	const BigUnsigned::Blk  iMask = uiMask >> 1;
	const BigUnsigned::Blk usMask = (unsigned short)(~0);
	const BigUnsigned::Blk  sMask = usMask >> 1;
}

BigInteger::operator unsigned long() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1)
			return *blk;
		else
			throw "BigInteger operator unsigned long() const: Value is too big for an unsigned long";
		case negative:
		throw "BigInteger operator unsigned long() const: Cannot convert a negative integer to an unsigned type";
		default:
		throw "BigInteger: Internal error";
	}
}

BigInteger::operator long() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1 && (*blk & ~lMask) == 0)
			return long(*blk);
		else
			throw "BigInteger operator long() const: Value is too big for a long";
		case negative:
		if (len == 1 && (*blk & ~lMask) == 0)
			return -long(*blk);
		else
			throw "BigInteger operator long() const: Value is too big for a long";
		default:
		throw "BigInteger: Internal error";
	}
}

BigInteger::operator unsigned int() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1 && (*blk & ~uiMask) == 0)
			return (unsigned int)(*blk);
		else
			throw "BigInteger operator unsigned int() const: Value is too big for an unsigned int";
		case negative:
		throw "BigInteger operator unsigned int() const: Cannot convert a negative integer to an unsigned type";
		default:
		throw "BigInteger: Internal error";
	}
}

BigInteger::operator int() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1 && (*blk & ~iMask) == 0)
			return int(*blk);
		else
			throw "BigInteger operator int() const: Value is too big for an int";
		case negative:
		if (len == 1 && (*blk & ~iMask) == 0)
			return -int(*blk);
		else
			throw "BigInteger operator int() const: Value is too big for an int";
		default:
		throw "BigInteger: Internal error";
	}
}

BigInteger::operator unsigned short() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1 && (*blk & ~usMask) == 0)
			return (unsigned short)(*blk);
		else
			throw "BigInteger operator unsigned short() const: Value is too big for an unsigned short";
		case negative:
		throw "BigInteger operator unsigned short() const: Cannot convert a negative integer to an unsigned type";
		default:
		throw "BigInteger: Internal error";
	}
}

BigInteger::operator short() const {
	switch (sign) {
		case zero:
		return 0;
		case positive:
		if (len == 1 && (*blk & ~sMask) == 0)
			return short(*blk);
		else
			throw "BigInteger operator short() const: Value is too big for a short";
		case negative:
		if (len == 1 && (*blk & ~sMask) == 0)
			return -short(*blk);
		else
			throw "BigInteger operator short() const: Value is too big for a short";
		default:
		throw "BigInteger: Internal error";
	}
}

// COMPARISON
BigInteger::CmpRes BigInteger::compareTo(const BigInteger &x) const {
	// A greater sign implies a greater number
	if (sign < x.sign)
		return less;
	else if (sign > x.sign)
		return greater;
	else switch (sign) {
		// If the signs are the same...
		case zero:
		return equal; // Two zeros are equal
		case positive:
		// Compare the magnitudes
		return BigUnsigned::compareTo(x);
		case negative:
		// Compare the magnitudes, but return the opposite result
		return CmpRes(-BigUnsigned::compareTo(x));
		default:
		throw "BigInteger: Internal error";
	}
}

// PUT-HERE OPERATIONS
// These do some messing around to determine the sign of the result,
// then call one of BigUnsigned's put-heres.

// Addition
void BigInteger::add(const BigInteger &a, const BigInteger &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigInteger::add: One of the arguments is the invoked object";
	// If one argument is zero, copy the other.
	if (a.sign == zero)
		operator =(b);
	else if (b.sign == zero)
		operator =(a);
	// If the arguments have the same sign, take the
	// common sign and add their magnitudes.
	else if (a.sign == b.sign) {
		sign = a.sign;
		BigUnsigned::add(a, b);
	} else {
		// Otherwise, their magnitudes must be compared.
		switch (a.BigUnsigned::compareTo(b)) {
			// If their magnitudes are the same, copy zero.
			case equal:
			len = 0;
			sign = zero;
			break;
			// Otherwise, take the sign of the greater, and subtract
			// the lesser magnitude from the greater magnitude.
			case greater:
			sign = a.sign;
			BigUnsigned::subtract(a, b);
			break;
			case less:
			sign = b.sign;
			BigUnsigned::subtract(b, a);
			break;
		}
	}
}

// Subtraction
void BigInteger::subtract(const BigInteger &a, const BigInteger &b) {
	// Notice that this routine is identical to BigInteger::add,
	// if one replaces b.sign by its opposite.
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigInteger::subtract: One of the arguments is the invoked object";
	// If a is zero, copy b and flip its sign.  If b is zero, copy a.
	if (a.sign == zero) {
		BigUnsigned::operator =(b);
		sign = Sign(-sign);
	} else if (b.sign == zero)
    operator =(a);
	// If their signs differ, take a.sign and add the magnitudes.
	else if (a.sign != b.sign) {
		sign = a.sign;
		BigUnsigned::add(a, b);
	} else {
		// Otherwise, their magnitudes must be compared.
		switch (a.BigUnsigned::compareTo(b)) {
			// If their magnitudes are the same, copy zero.
			case equal:
			len = 0;
			sign = zero;
			break;
			// If a's magnitude is greater, take a.sign and
			// subtract a from b.
			case greater:
			sign = a.sign;
			BigUnsigned::subtract(a, b);
			break;
			// If b's magnitude is greater, take the opposite
			// of b.sign and subtract b from a.
			case less:
			sign = Sign(-b.sign);
			BigUnsigned::subtract(b, a);
			break;
		}
	}
}

// Multiplication
void BigInteger::multiply(const BigInteger &a, const BigInteger &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigInteger::multiply: One of the arguments is the invoked object";
	// If one object is zero, copy zero and return.
	if (a.sign == zero || b.sign == zero) {
		sign = zero;
		len = 0;
		return;
	}
	// If the signs of the arguments are the same, the result
	// is positive, otherwise it is negative.
	sign = (a.sign == b.sign) ? positive : negative;
	// Multiply the magnitudes.
	BigUnsigned::multiply(a, b);
}

// Division
void BigInteger::divide(const BigInteger &a, const BigInteger &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigInteger::divide: One of the arguments is the invoked object";
	// If b is zero, the caller has tried to divide by zero.  Throw an exception.
	if (b.sign == zero)
		throw "BigInteger::divide: Division by zero";
	// Otherwise if a is zero, copy zero and return.
	else if (a.sign == zero) {
		sign = zero;
		len = 0;
		return;
	}
	// If the signs of the arguments are the same, the result
	// is positive, otherwise it is negative.
	sign = (a.sign == b.sign) ? positive : negative;
	// Divide the magnitudes.
	// Note: This is integer division.  Any fractional part
	// of the result is truncated toward zero.
	BigUnsigned::divide(a, b);
	// If the result is zero, set the sign to zero.
	if (len == 0)
		sign = zero;
}

// Modular reduction
void BigInteger::modulo(const BigInteger &a, const BigInteger &b) {
	/* Note that the mathematical definition of mod is somewhat
	* different from the way the normal C++ % operator behaves.
	* This function does it the mathematical way. */
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigInteger::modulo: One of the arguments is the invoked object";
	// If b is zero, copy a and return.  By the mathematical definition,
	// x mod 0 = x, though the normal C++ % would throw an exception.
	if (b.len == 0) {
		operator =(a);
		return;
		// If a is zero, copy zero and return.
	} else if (a.sign == zero) {
		sign = zero;
		len = 0;
		return;
	}
	// Perform modular reduction on the magnitudes
	BigUnsigned::modulo(a, b);
	// If the result is zero, set the sign to zero.
	if (len == 0)
		sign = zero;
	else {
		/* If necessary, flip the result over zero so that it has the
		* same sign as the modulus (by the mathematical definition).
		* The normal C++ % does not perform this step and always
		* takes the sign of the first input. */
		if (a.sign != b.sign) {
			BigUnsigned temp(*this);
			BigUnsigned::subtract(b, temp);
		}
		sign = b.sign;
	}
}

// Negation
void BigInteger::negate(const BigInteger &a) {
	// Block unsafe calls
	if (this == &a)
		throw "BigInteger::negate: The argument is the invoked object";
	// Copy a's magnitude
	BigUnsigned::operator =(a);
	// Copy the opposite of a.sign
	sign = Sign(-a.sign);
}

// INCREMENT/DECREMENT OPERATORS

// Prefix increment
void BigInteger::operator ++() {
	switch (sign) {
		case zero:
		allocate(1);
		sign = positive;
		len = 1;
		*blk = 1;
		break;
		case positive:
		BigUnsigned::operator ++();
		break;
		case negative:
		BigUnsigned::operator --();
		if (len == 0)
			sign = zero;
		break;
	}
}

// Postfix increment: same as prefix
void BigInteger::operator ++(int) {
	operator ++();
}

// Prefix decrement
void BigInteger::operator --() {
	switch (sign) {
		case zero:
		allocate(1);
		sign = negative;
		len = 1;
		*blk = 1;
		break;
		case negative:
		BigUnsigned::operator ++();
		break;
		case positive:
		BigUnsigned::operator --();
		if (len == 0)
			sign = zero;
		break;
	}
}

// Postfix decrement: same as prefix
void BigInteger::operator --(int) {
	operator --();
}

