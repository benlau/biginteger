/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#include "BigUnsigned.hh"

// The "management" routines that used to be here are now in NumberlikeArray.hh.

/*
* The steps for construction of a BigUnsigned
* from an integral value x are as follows:
* 1. If x is zero, create an empty BigUnsigned and stop.
* 2. If x is negative, throw an exception.
* 3. Allocate a one-block number array.
* 4. If x is of a signed type, convert x to the unsigned
*    type of the same length.
* 5. Expand x to a Blk, and store it in the number array.
*
* Since 2005.01.06, NumberlikeArray uses `NULL' rather
* than a real array if one of zero length is needed.
* These constructors implicitly call NumberlikeArray's
* default constructor, which sets `blk = NULL, cap = len = 0'.
* So if the input number is zero, they can just return.
* See remarks in `NumberlikeArray.hh'.
*/

BigUnsigned::BigUnsigned(unsigned long x) {
	if (x == 0)
		; // NumberlikeArray already did all the work
	else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	}
}

BigUnsigned::BigUnsigned(long x) {
	if (x == 0)
		;
	else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	} else
	throw "BigUnsigned::BigUnsigned(long): Cannot construct a BigUnsigned from a negative number";
}

BigUnsigned::BigUnsigned(unsigned int x) {
	if (x == 0)
		;
	else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	}
}

BigUnsigned::BigUnsigned(int x) {
	if (x == 0)
		;
	else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	} else
	throw "BigUnsigned::BigUnsigned(int): Cannot construct a BigUnsigned from a negative number";
}

BigUnsigned::BigUnsigned(unsigned short x) {
	if (x == 0)
		;
	else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	}
}

BigUnsigned::BigUnsigned(short x) {
	if (x == 0)
		;
	else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		blk[0] = Blk(x);
	} else
	throw "BigUnsigned::BigUnsigned(short): Cannot construct a BigUnsigned from a negative number";
}

// CONVERTERS
/*
* The steps for conversion of a BigUnsigned to an
* integral type are as follows:
* 1. If the BigUnsigned is zero, return zero.
* 2. If it is more than one block long or its lowest
*    block has bits set out of the range of the target
*    type, throw an exception.
* 3. Otherwise, convert the lowest block to the
*    target type and return it.
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

BigUnsigned::operator unsigned long() const {
	if (len == 0)
		return 0;
	else if (len == 1)
		return (unsigned long) blk[0];
	else
		throw "BigUnsigned::operator unsigned long: Value is too big for an unsigned long";
}

BigUnsigned::operator long() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (blk[0] & lMask) == blk[0])
		return (long) blk[0];
	else
		throw "BigUnsigned::operator long: Value is too big for a long";
}

BigUnsigned::operator unsigned int() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (blk[0] & uiMask) == blk[0])
		return (unsigned int) blk[0];
	else
		throw "BigUnsigned::operator unsigned int: Value is too big for an unsigned int";
}

BigUnsigned::operator int() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (blk[0] & iMask) == blk[0])
		return (int) blk[0];
	else
		throw "BigUnsigned::operator int: Value is too big for an int";
}

BigUnsigned::operator unsigned short() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (blk[0] & usMask) == blk[0])
		return (unsigned short) blk[0];
	else
		throw "BigUnsigned::operator unsigned short: Value is too big for an unsigned short";
}

BigUnsigned::operator short() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (blk[0] & sMask) == blk[0])
		return (short) blk[0];
	else
		throw "BigUnsigned::operator short: Value is too big for a short";
}

// COMPARISON
BigUnsigned::CmpRes BigUnsigned::compareTo(const BigUnsigned &x) const {
	// A bigger length implies a bigger number.
	if (len < x.len)
		return less;
	else if (len > x.len)
		return greater;
	else {
		// Compare blocks one by one from left to right.
		Index i = len;
		while (i > 0) {
			i--;
			if (blk[i] == x.blk[i])
				continue;
			else if (blk[i] > x.blk[i])
				return greater;
			else
				return less;
		}
		// If no blocks differed, the numbers are equal.
		return equal;
	}
}

// PUT-HERE OPERATIONS

// Addition
void BigUnsigned::add(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::add: One of the arguments is the invoked object";
	// If one argument is zero, copy the other.
	if (a.len == 0) {
		operator =(b);
		return;
	} else if (b.len == 0) {
		operator =(a);
		return;
	}
	// Carries in and out of an addition stage
	bool carryIn, carryOut;
	Blk temp;
	Index i;
	// a2 points to the longer input, b2 points to the shorter
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	// Set prelimiary length and make room in this BigUnsigned
	len = a2->len + 1;
	allocate(len);
	// For each block index that is present in both inputs...
	for (i = 0, carryIn = false; i < b2->len; i++) {
		// Add input blocks
		temp = a2->blk[i] + b2->blk[i];
		// If a rollover occurred, the result is less than either input.
		// This test is used many times in the BigUnsigned code.
		carryOut = (temp < a2->blk[i]);
		// If a carry was input, handle it
		if (carryIn) {
			temp++;
			carryOut |= (temp == 0);
		}
		blk[i] = temp; // Save the addition result
		carryIn = carryOut; // Pass the carry along
	}
	// If there is a carry left over, increase blocks until
	// one does not roll over.
	for (; i < a2->len && carryIn; i++) {
		temp = a2->blk[i] + 1;
		carryIn = (temp == 0);
		blk[i] = temp;
	}
	// If the carry was resolved but the larger number
	// still has blocks, copy them over.
	for (; i < a2->len; i++)
		blk[i] = a2->blk[i];
	// Set the extra block if there's still a carry, decrease length otherwise
	if (carryIn)
		blk[i] = 1;
	else
		len--;
}

// Subtraction
void BigUnsigned::subtract(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::subtract: One of the arguments is the invoked object";
	// If b is zero, copy a.  If a is shorter than b, the result is negative.
	if (b.len == 0) {
		operator =(a);
		return;
	} else if (a.len < b.len)
	throw "BigUnsigned::subtract: Negative result in unsigned calculation";
	bool borrowIn, borrowOut;
	Blk temp;
	Index i;
	// Set preliminary length and make room
	len = a.len;
	allocate(len);
	// For each block index that is present in both inputs...
	for (i = 0, borrowIn = false; i < b.len; i++) {
		temp = a.blk[i] - b.blk[i];
		// If a reverse rollover occurred, the result is greater than the block from a.
		borrowOut = (temp > a.blk[i]);
		// Handle an incoming borrow
		if (borrowIn) {
			borrowOut |= (temp == 0);
			temp--;
		}
		blk[i] = temp; // Save the subtraction result
		borrowIn = borrowOut; // Pass the borrow along
	}
	// If there is a borrow left over, decrease blocks until
	// one does not reverse rollover.
	for (; i < a.len && borrowIn; i++) {
		borrowIn = (a.blk[i] == 0);
		blk[i] = a.blk[i] - 1;
	}
	// If there's still a borrow, the result is negative.
	// Throw an exception, but zero out this object first just in case.
	if (borrowIn) {
		len = 0;
		throw "BigUnsigned::subtract: Negative result in unsigned calculation";
	} else // Copy over the rest of the blocks
	for (; i < a.len; i++)
		blk[i] = a.blk[i];
	// Zap leading zeros
	zapLeadingZeros();
}

// Multiplication
void BigUnsigned::multiply(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::multiply: One of the arguments is the invoked object";
	// If either a or b is zero, set to zero.
	if (a.len == 0 || b.len == 0) {
		len = 0;
		return;
	}
	// Overall method: this = 0, then for each 1-bit of a, add b
	// to this shifted the appropriate amount.
	// Variables for the calculation
	Index i, j, k;
	unsigned int i2;
	Blk aBlk, bHigh, temp;
	bool carryIn, carryOut;
	// Set preliminary length and make room
	len = a.len + b.len;
	allocate(len);
	// Zero out this object
	for (i = 0; i < len; i++)
		blk[i] = 0;
	// For each block of the first number...
	for (i = 0; i < a.len; i++) {
		// For each 1-bit of that block...
		for (i2 = 0, aBlk = a.blk[i]; aBlk != 0; i2++, aBlk >>= 1) {
			if ((aBlk & 1) == 0)
				continue;
			/* Add b to this, shifted left i blocks and i2 bits.
			* j is the index in b, and k = i + j is the index in this.
			* The low bits of b.blk[j] are shifted and added to blk[k].
			* bHigh is used to carry the high bits to the next addition. */
			bHigh = 0;
			for (j = 0, k = i, carryIn = false; j < b.len; j++, k++) {
				temp = blk[k] + ((b.blk[j] << i2) | bHigh);
				carryOut = (temp < blk[k]);
				if (carryIn) {
					temp++;
					carryOut |= (temp == 0);
				}
				blk[k] = temp;
				carryIn = carryOut;
				bHigh = (i2 == 0) ? 0 : b.blk[j] >> (8 * sizeof(Blk) - i2);
			}
			temp = blk[k] + bHigh;
			carryOut = (temp < blk[k]);
			if (carryIn) {
				temp++;
				carryOut |= (temp == 0);
			}
			blk[k] = temp;
			carryIn = carryOut;
			k++; // Added by Matt 2004.12.23: Move to the next block.  It belongs here (and there was a corresponding line in the division routine), but I'm not certain whether it ever matters.
			for (; carryIn; k++) {
				blk[k]++;
				carryIn = (blk[k] == 0);
			}
		}
	}
	// Zap possible leading zero
	if (blk[len - 1] == 0)
		len--;
}

/*
* DIVISION WITH REMAINDER
* The functionality of divide, modulo, and %= is included in this one monstrous call,
* which deserves some explanation.
*
* The division *this / b is performed.
* Afterwards, q has the quotient, and *this has the remainder.
* Thus, a call is like q = *this / b, *this %= b.
*
* This seemingly bizarre pattern of inputs and outputs has a justification.  The
* ``put-here operations'' are supposed to be fast.  Therefore, they accept inputs
* and provide outputs in the most convenient places so that no value ever needs
* to be copied in its entirety.  That way, the client can perform exactly the
* copying it needs depending on where the inputs are and where it wants the output.
*/
void BigUnsigned::divideWithRemainder(const BigUnsigned &b, BigUnsigned &q) {
	// Block unsafe calls
	if (this == &b || &q == &b || this == &q)
		throw "BigUnsigned::divideWithRemainder: Some two objects involved are the same";
	
	/*
	* Note that the mathematical definition of mod (I'm trusting Knuth) is somewhat
	* different from the way the normal C++ % operator behaves in the case of division by 0.
	* This function does it Knuth's way.
	*
	* We let a / 0 == 0 (it doesn't matter) and a % 0 == a, no exceptions thrown.
	* This allows us to preserve both Knuth's demand that a mod 0 == a
	* and the useful property that (a / b) * b + (a % b) == a.
	*/
	if (b.len == 0) {
		q.len = 0;
		return;
	}
	
	/*
	* If *this.len < b.len, then *this < b, and we can be sure that b doesn't go into
	* *this at all.  The quotient is 0 and *this is already the remainder (so leave it alone).
	*/
	if (len < b.len) {
		q.len = 0;
		return;
	}
	
	/*
	* At this point we know *this > b > 0.  (Whew!)
	*/
	
	/* DEBUG *
	std::cout << "divideWithRemainder starting\n"
	<< "length of dividend: " << len
	<< "\nlast block of dividend: " << getBlock(0)
	<< "\nlength of divisor: " << b.len
	<< "\nlast block of divisor: " << b.getBlock(0)
	<< std::endl; */
	
	/*
	* Overall method: Subtract b, shifted varying amounts to
	* the left, from this, setting the bit in the quotient q
	* whenever the subtraction succeeds.  Eventually q will contain the entire
	* quotient, and this will be left with the remainder.
	*
	* We use work2 to temporarily store the result of a subtraction.
	* But we don't even compute the i lowest blocks of the result,
	* because they are unaffected (we shift left i places).
	* */
	// Variables for the calculation
	Index i, j, k;
	unsigned int i2;
	Blk bHigh, temp;
	bool borrowIn, borrowOut;
	
	// Make sure we have an extra zero block just past the value,
	// but don't increase the logical length.  A shifted subtraction
	// (for example, subtracting 1 << 2 from 4) might stick into
	// this block.
	allocateAndCopy(len + 1);
	blk[len] = 0;
	
	// work2 holds part of the result of a subtraction.
	// (There's no work1.  The name work2 is from a previous version.)
	Blk *work2 = new Blk[len];
	
	// Set preliminary length for quotient and make room
	q.len = len - b.len + 1;
	q.allocate(q.len);
	// Zero out the quotient
	for (i = 0; i < q.len; i++)
		q.blk[i] = 0;
	
	// For each possible left-shift of b in blocks...
	i = q.len;
	while (i > 0) {
		i--;
		// For each possible left-shift of b in bits...
		q.blk[i] = 0;
		i2 = 8 * sizeof(Blk);
		while (i2 > 0) {
			i2--;
			/*
			* Subtract b, shifted left i blocks and i2 bits, from this.
			* and store the answer in work2.
			*
			* Compare this to the middle section of `multiply'.  They
			* are in many ways analogous.
			*/
			bHigh = 0;
			for (j = 0, k = i, borrowIn = false; j < b.len; j++, k++) {
				temp = blk[k] - ((b.blk[j] << i2) | bHigh);
				borrowOut = (temp > blk[k]);
				if (borrowIn) {
					borrowOut |= (temp == 0);
					temp--;
				}
				work2[j] = temp;
				borrowIn = borrowOut;
				bHigh = (i2 == 0) ? 0 : b.blk[j] >> (8 * sizeof(Blk) - i2);
			}
			temp = blk[k] - bHigh;
			borrowOut = (temp > blk[k]);
			if (borrowIn) {
				borrowOut |= (temp == 0);
				temp--;
			}
			work2[j] = temp;
			borrowIn = borrowOut;
			j++;
			k++;
			for (; k < len && borrowIn; j++, k++) {
				borrowIn = (blk[k] == 0);
				work2[j] = blk[k] - 1;
			}
			/* If the subtraction was performed successfully (!borrowIn), set bit i2
			* in block i of the quotient, and copy the changed portion of
			* work2 back to this. Otherwise, reset that bit and move on. */
			if (!borrowIn) {
				q.blk[i] |= (1 << i2);
				while (j > 0) {
					j--;
					k--;
					blk[k] = work2[j];
				}
			} 
		}
	}
	// Zap possible leading zero in quotient
	if (q.blk[q.len - 1] == 0)
		q.len--;
	// Zap any/all leading zeros in remainder
	zapLeadingZeros();
	// Deallocate temporary array.
	// (Thanks to Brad Spencer for noticing my accidental omission of this!)
	delete [] work2;
	
	/* DEBUG *
	std::cout << "divideWithRemainder complete\n"
	<< "length of quotient: " << q.len
	<< "\nlast block of quotient: " << q.getBlock(0)
	<< "\nlength of remainder: " << len
	<< "\nlast block of remainder: " << getBlock(0)
	<< std::endl; */
}

// Bitwise and
void BigUnsigned::bitAnd(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitAnd: One of the arguments is the invoked object";
	len = (a.len >= b.len) ? b.len : a.len;
	allocate(len);
	Index i;
	for (i = 0; i < len; i++)
		blk[i] = a.blk[i] & b.blk[i];
	zapLeadingZeros();
}

// Bitwise or
void BigUnsigned::bitOr(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitOr: One of the arguments is the invoked object";
	Index i;
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	allocate(a2->len);
	for (i = 0; i < b2->len; i++)
		blk[i] = a2->blk[i] | b2->blk[i];
	for (; i < a2->len; i++)
		blk[i] = a2->blk[i];
	len = a2->len;
}

// Bitwise xor
void BigUnsigned::bitXor(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitXor: One of the arguments is the invoked object";
	Index i;
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	allocate(b2->len);
	for (i = 0; i < b2->len; i++)
		blk[i] = a2->blk[i] ^ b2->blk[i];
	for (; i < a2->len; i++)
		blk[i] = a2->blk[i];
	len = a2->len;
	zapLeadingZeros();
}

// INCREMENT/DECREMENT OPERATORS

// Prefix increment
void BigUnsigned::operator ++() {
	Index i;
	bool carry = true;
	for (i = 0; i < len && carry; i++) {
		blk[i]++;
		carry = (blk[i] == 0);
	}
	if (carry) {
		// Matt fixed a bug 2004.12.24: next 2 lines used to say allocateAndCopy(len + 1)
		len++;
		allocateAndCopy(len);
		blk[i] = 1;
	}
}

// Postfix increment: same as prefix
void BigUnsigned::operator ++(int) {
	operator ++();
}

// Prefix decrement
void BigUnsigned::operator --() {
	if (len == 0)
		throw "BigUnsigned::operator --(): Cannot decrement an unsigned zero";
	Index i;
	bool borrow = true;
	for (i = 0; borrow; i++) {
		borrow = (blk[i] == 0);
		blk[i]--;
	}
	// Zap possible leading zero (there can only be one)
	if (blk[len - 1] == 0)
		len--;
}

// Postfix decrement: same as prefix
void BigUnsigned::operator --(int) {
	operator --();
}
