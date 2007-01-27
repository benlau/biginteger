/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#include "BigUnsigned.h"

// MANAGEMENT

// This routine is called to ensure the number array is at least a
// certain size before the result of an operation is written over it. 
void BigUnsigned::allocate(BlkNum c) {
	// If the requested capacity is more than the current capacity...
	if (c > cap) {
		// Delete the old number array
		delete [] blk;
		// Allocate the new array
		cap = c;
		blk = new Blk[cap];
	}
}

// This routine is called to ensure the number array is at least a
// certain size without losing its contents.
void BigUnsigned::allocateAndCopy(BlkNum c) {
	// If the requested capacity is more than the current capacity...
	if (c > cap) {
		Blk *oldBlk = blk;
		// Allocate the new number array
		cap = c;
		blk = new Blk[cap];
		// Copy number blocks
		BlkNum i;
		Blk *blkI;
		const Blk *oldBlkI;
		for (i = 0, blkI = blk, oldBlkI = oldBlk; i < len; i++, blkI++, oldBlkI++)
			*blkI = *oldBlkI;
		// Delete the old array
		delete [] oldBlk;
	}
}

// Copy constructor
BigUnsigned::BigUnsigned(const BigUnsigned &x) : len(x.len) {
	// Create number array
	cap = len;
	blk = new Blk[cap];
	// Copy number blocks
	BlkNum i;
	Blk *blkI;
	const Blk *xBlkI;
	for (i = 0, blkI = blk, xBlkI = x.blk; i < len; i++, blkI++, xBlkI++)
		*blkI = *xBlkI;
}

// Assignment operator
void BigUnsigned::operator=(const BigUnsigned &x) {
	// Calls like a = a have no effect
	if (this == &x)
		return;
	// Copy length
	len = x.len;
	// Expand number array if necessary
	allocate(len);
	// Copy number blocks
	BlkNum i;
	Blk *blkI;
	const Blk *xBlkI;
	for (i = 0, blkI = blk, xBlkI = x.blk; i < len; i++, blkI++, xBlkI++)
		*blkI = *xBlkI;
}

// Constructor from an array of blocks
BigUnsigned::BigUnsigned(const Blk *b, BlkNum l) : cap(l), len(l) {
	// Create number array
	blk = new Blk[cap];
	// Copy number blocks
	BlkNum i;
	Blk *blkI;
	const Blk *bI;
	for (i = 0, blkI = blk, bI = b; i < len; i++, blkI++, bI++)
		*blkI = *bI;
	zapLeadingZeros();
}

/*
* The steps for construction of a BigUnsigned
* from an integral value x are as follows:
* 1. If x is zero, create an empty BigUnsigned and stop.
* 2. If x is negative, throw an exception.
* 3. Allocate a one-block number array.
* 4. If x is of a signed type, convert x to the unsigned
*    type of the same length.
* 5. Expand x to a Blk, and store it in the number array.
*/

BigUnsigned::BigUnsigned(unsigned long x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
	}
}

BigUnsigned::BigUnsigned(long x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
	} else
    throw "BigUnsigned::BigUnsigned(long): Cannot construct a BigUnsigned from a negative number";
}

BigUnsigned::BigUnsigned(unsigned int x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
	}
}

BigUnsigned::BigUnsigned(int x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
	} else
    throw "BigUnsigned::BigUnsigned(int): Cannot construct a BigUnsigned from a negative number";
}

BigUnsigned::BigUnsigned(unsigned short x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
	}
}

BigUnsigned::BigUnsigned(short x) {
	if (x == 0) {
		cap = 0;
		blk = new Blk[0];
		len = 0;
	} else if (x > 0) {
		cap = 1;
		blk = new Blk[1];
		len = 1;
		*blk = Blk(x);
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
		return (unsigned long) *blk;
	else
		throw "BigUnsigned::operator unsigned long: Value is too big for an unsigned long";
}

BigUnsigned::operator long() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (*blk & lMask) == *blk)
		return (long) *blk;
	else
		throw "BigUnsigned::operator long: Value is too big for a long";
}

BigUnsigned::operator unsigned int() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (*blk & uiMask) == *blk)
		return (unsigned int) *blk;
	else
		throw "BigUnsigned::operator unsigned int: Value is too big for an unsigned int";
}

BigUnsigned::operator int() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (*blk & iMask) == *blk)
		return (int) *blk;
	else
		throw "BigUnsigned::operator int: Value is too big for an int";
}

BigUnsigned::operator unsigned short() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (*blk & usMask) == *blk)
		return (unsigned short) *blk;
	else
		throw "BigUnsigned::operator unsigned short: Value is too big for an unsigned short";
}

BigUnsigned::operator short() const {
	if (len == 0)
		return 0;
	else if (len == 1 && (*blk & sMask) == *blk)
		return (short) *blk;
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
		BlkNum i = len;
		const Blk *blkI = blk + len;
		const Blk *xBlkI = x.blk + len;
		while (i > 0) {
			i--;
			blkI--;
			xBlkI--;
			if (*blkI == *xBlkI)
				continue;
			else if (*blkI > *xBlkI)
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
	BlkNum i;
	// a2 points to the longer input, b2 points to the shorter
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	// These point directly to the position of interest in the
	// corresponding block arrays, for faster access.
	Blk *blkI;
	const Blk *a2BlkI, *b2BlkI;
	// Set prelimiary length and make room in this BigUnsigned
	len = a2->len + 1;
	allocate(len);
	// For each block index that is present in both inputs...
	for (i = 0, blkI = blk, a2BlkI = a2->blk, b2BlkI = b2->blk,
	carryIn = false; i < b2->len; i++, blkI++, a2BlkI++, b2BlkI++) {
		// Add input blocks
		temp = *a2BlkI + *b2BlkI;
		// If a rollover occurred, the result is less than either input.
		// This test is used many times in the BigUnsigned code.
		carryOut = (temp < *a2BlkI);
		// If a carry was input, handle it
		if (carryIn) {
			temp++;
			carryOut |= (temp == 0);
		}
		*blkI = temp; // Save the addition result
		carryIn = carryOut; // Pass the carry along
	}
	// If there is a carry left over, increase blocks until
	// one does not roll over.
	for (; i < a2->len && carryIn; i++, blkI++, a2BlkI++) {
		temp = *a2BlkI + 1;
		carryIn = (temp == 0);
		*blkI = temp;
	}
	// If the carry was resolved but the larger number
	// still has blocks, copy them over.
	for (; i < a2->len; i++, blkI++, a2BlkI++)
		*blkI = *a2BlkI;
	// Set the extra block if there's still a carry, decrease length otherwise
	if (carryIn)
		*blkI = 1;
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
	BlkNum i;
	// These point directly to the position of interest in the
	// corresponding block arrays, for faster access.
	Blk *blkI;
	const Blk *aBlkI, *bBlkI;
	// Set preliminary length and make room
	len = a.len;
	allocate(len);
	// For each block index that is present in both inputs...
	for (i = 0, blkI = blk, aBlkI = a.blk, bBlkI = b.blk,
	borrowIn = false; i < b.len; i++, blkI++, aBlkI++, bBlkI++) {
		temp = *aBlkI - *bBlkI;
		// If a reverse rollover occurred, the result is greater than the block from a.
		borrowOut = (temp > *aBlkI);
		// Handle an incoming borrow
		if (borrowIn) {
			borrowOut |= (temp == 0);
			temp--;
		}
		*blkI = temp; // Save the subtraction result
		borrowIn = borrowOut; // Pass the borrow along
	}
	// If there is a borrow left over, decrease blocks until
	// one does not reverse rollover.
	for (; i < a.len && borrowIn; i++, blkI++, aBlkI++) {
		borrowIn = (*aBlkI == 0);
		*blkI = *aBlkI - 1;
	}
	// If there's still a borrow, the result is negative.
	// Throw an exception, but zero out this object first.
	if (borrowIn) {
		len = 0;
		throw "BigUnsigned::subtract: Negative result in unsigned calculation";
	} else // Copy over the rest of the blocks
    for (; i < a.len; i++, blkI++, aBlkI++)
		*blkI = *aBlkI;
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
	BlkNum i, j;
	unsigned int i2;
	Blk aBlk, bHigh, temp;
	bool carryIn, carryOut;
	// These point directly to the positions of interest in the
	// corresponding block arrays, for faster access.
	Blk *blkI, *blkK;
	const Blk *aBlkI, *bBlkJ;
	// Set preliminary length and make room
	len = a.len + b.len;
	allocate(len);
	// Zero out this object
	for (i = 0, blkI = blk; i < len; i++, blkI++)
		*blkI = 0;
	// For each block of the first number...
	for (i = 0, blkI = blk, aBlkI = a.blk; i < a.len; i++, blkI++, aBlkI++)
		// For each 1-bit of that block...
    for (i2 = 0, aBlk = *aBlkI; aBlk != 0; i2++, aBlk >>= 1) {
		if ((aBlk & 1) == 0)
			continue;
		/* Add b to this, shifted left i blocks and i2 bits.
		* j is the index in b, and k = i + j is the index in this.
		* The low bits of b.blk[j] are shifted and added to blk[k].
		* bHigh is used to carry the high bits to the next addition. */
		carryIn = false;
		bHigh = 0;
		for (j = 0, bBlkJ = b.blk, blkK = blkI; j < b.len; j++, bBlkJ++, blkK++) {
			temp = *blkK + ((*bBlkJ << i2) | bHigh);
			carryOut = (temp < *blkK);
			if (carryIn) {
				temp++;
				carryOut |= (temp == 0);
			}
			*blkK = temp;
			carryIn = carryOut;
			bHigh = (i2 == 0) ? 0 : *bBlkJ >> (8 * sizeof(Blk) - i2);
		}
		temp = *blkK + bHigh;
		carryOut = (temp < *blkK);
		if (carryIn) {
			temp++;
			carryOut |= (temp == 0);
		}
		*blkK = temp;
		carryIn = carryOut;
		for (; carryIn; blkK++) {
			(*blkK)++;
			carryIn = (*blkK == 0);
		}
    }
	// Zap leading zero
	if (blk[len - 1] == 0)
		len--;
}

// Division
void BigUnsigned::divide(const BigUnsigned &a, const BigUnsigned &b) {
	// Note: This is integer division.  Any fractional part
	// of the result is truncated.
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::divide: One of the arguments is the invoked object";
	// If b is zero, throw an exception.  If a is zero, set to zero.
	else if (b.len == 0)
		throw "BigUnsigned::divide: Division by zero";
	else if (a.len < b.len) {
		len = 0;
		return;
	}
	/* Overall method: Subtract b, shifted varying amounts to
	* the left, from a, setting the bit in the quotient
	* whenever the subtraction succeeds. */
	// Variables for the calculation
	BlkNum i, j, k;
	unsigned int i2;
	Blk bHigh, temp;
	bool borrowIn, borrowOut;
	// work1 is a copy of a that can be modified
	// after each successful subtraction.
	Blk *work1 = new Blk[a.len + 1];
	Blk *work1I = work1;
	const Blk *aBlkI = a.blk;
	for (i = 0; i < a.len; i++, work1I++, aBlkI++)
		*work1I = *aBlkI;
	*work1I = 0;
	// work2 holds part of the result of a subtraction
	Blk *work2 = new Blk[a.len + 1];
	// These point directly to the positions of interest in the
	// corresponding block arrays, for faster access.
	Blk *blkI, *work1K, *work2J;
	const Blk *bBlkJ;
	// Set preliminary length and make room
	len = a.len - b.len + 1;
	allocate(len);
	// For each possible left-shift of b in blocks...
	i = len;
	blkI = blk + len;
	work1I = work1 + len;
	while (i > 0) {
		i--;
		blkI--;
		work1I--;
		// For each possible left-shift of b in bits...
		*blkI = 0;
		i2 = 8 * sizeof(Blk);
		while (i2 > 0) {
			i2--;
			// Subtract b, shifted left i blocks and i2 bits, from work1
			// and store the answer in work2.  See note in BigUnsigned::multiply.
			bHigh = 0;
			for (j = 0, bBlkJ = b.blk, work2J = work2, k = i, work1K = work1I,
			borrowIn = false; j < b.len; j++, bBlkJ++, work2J++, k++, work1K++) {
				temp = *work1K - ((*bBlkJ << i2) | bHigh);
				borrowOut = (temp > *work1K);
				if (borrowIn) {
					borrowOut |= (temp == 0);
					temp--;
				}
				*work2J = temp;
				borrowIn = borrowOut;
				bHigh = (i2 == 0) ? 0 : *bBlkJ >> (8 * sizeof(Blk) - i2);
			}
			temp = *work1K - bHigh;
			borrowOut = (temp > *work1K);
			if (borrowIn) {
				borrowOut |= (temp == 0);
				temp--;
			}
			*work2J = temp;
			borrowIn = borrowOut;
			j++;
			work2J++;
			k++;
			work1K++;
			for (; k < a.len + 1 && borrowIn; j++, work2J++, k++, work1K++) {
				borrowIn = (*work1K == 0);
				*work2J = *work1K - 1;
			}
			/* If the subtraction was performed successfully, set bit i2
			* in block i of the quotient, and copy the changed portion of
			* work2 back to work1. Otherwise, reset that bit and move on. */
			if (!borrowIn) {
				*blkI |= (1 << i2);
				while (j > 0) {
					j--;
					work2J--;
					k--;
					work1K--;
					*work1K = *work2J;
				}
			}
		}
	}
	// Zap leading zero
	if (blk[len - 1] == 0)
		len--;
	// Deallocate temporary arrays.
	// (Thanks to Brad Spencer for noticing my accidental omission of this!)
	delete [] work1;
	delete [] work2;
}

// Modulo
void BigUnsigned::modulo(const BigUnsigned &a, const BigUnsigned &b) {
	/* Note that the mathematical definition of mod is somewhat
	* different from the way the normal C++ % operator behaves.
	* This function does it the mathematical way. */
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::modulo: One of the arguments is the invoked object";
	// If b is zero, copy a and return.  By the mathematical definition,
	// x mod 0 = x, though the normal C++ % would throw an exception.
	else if (b.len == 0) {
		len = 0;
		return;
		// If a is zero, set to zero and return.
	} else if (a.len < b.len) {
		operator =(a);
		return;
	}
	/* Overall method: Copy a into this.  Then subtract b,
	* shifted varying amounts to the left, from this.
	* When no more subtraction is possible, stop; this
	* will contain the remainder.
	* This is very similar to the division routine, except
	* that whether or not a subtraction succeeds is ignored,
	* and "this" serves the purpose of work1. */
	// Variables for the calculation
	BlkNum i, j, k;
	unsigned int i2;
	Blk bHigh, temp;
	bool borrowIn, borrowOut;
	allocate(a.len + 1);
	operator =(a);
	blk[len] = 0;
	// work2 holds part of the result of a subtraction
	Blk *work2 = new Blk[a.len + 1];
	// These point directly to the positions of interest in the
	// corresponding block arrays, for faster access.
	Blk *blkI, *blkK, *work2J;
	const Blk *bBlkJ;
	// For each possible left-shift of b in blocks...
	i = len;
	blkI = blk + len;
	while (i > 0) {
		i--;
		blkI--;
		// For each possible left-shift of b in bits...
		i2 = 8 * sizeof(Blk);
		while (i2 > 0) {
			i2--;
			// Subtract b, shifted left i blocks and i2 bits, from work1
			// and store the answer in work2.  See note in BigUnsigned::multiply.
			bHigh = 0;
			for (j = 0, bBlkJ = b.blk, work2J = work2, k = i, blkK = blkI,
			borrowIn = false; j < b.len; j++, bBlkJ++, work2J++, k++, blkK++) {
				temp = *blkK - ((*bBlkJ << i2) | bHigh);
				borrowOut = (temp > *blkK);
				if (borrowIn) {
					borrowOut |= (temp == 0);
					temp--;
				}
				*work2J = temp;
				borrowIn = borrowOut;
				bHigh = (i2 == 0) ? 0 : *bBlkJ >> (8 * sizeof(Blk) - i2);
			}
			temp = *blkK - bHigh;
			borrowOut = (temp > *blkK);
			if (borrowIn) {
				borrowOut |= (temp == 0);
				temp--;
			}
			*work2J = temp;
			borrowIn = borrowOut;
			j++;
			work2J++;
			k++;
			blkK++;
			for (; k < a.len + 1 && borrowIn; j++, work2J++, k++, blkK++) {
				borrowIn = (*blkK == 0);
				*work2J = *blkK - 1;
			}
			// If the subtraction was performed successfully, set bit i2
			// in block i of the quotient, and copy the changed portion of
			// work2 back to this.
			if (!borrowIn)
			while (j > 0) {
				j--;
				work2J--;
				k--;
				blkK--;
				*blkK = *work2J;
			}
		}
	}
	// Blocks higher than the last block of the modulus are zero.
	len = b.len;
	// Zap leading zeros
	zapLeadingZeros();
}

// Bitwise and
void BigUnsigned::bitAnd(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitAnd: One of the arguments is the invoked object";
	len = (a.len >= b.len) ? b.len : a.len;
	allocate(len);
	BlkNum i;
	Blk *blkI;
	const Blk *aBlkI, *bBlkI;
	for (i = 0, blkI = blk, aBlkI = a.blk, bBlkI = b.blk;
	i < len; i++, blkI++, aBlkI++, bBlkI++)
    *blkI = *aBlkI & *bBlkI;
	zapLeadingZeros();
}

// Bitwise or
void BigUnsigned::bitOr(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitOr: One of the arguments is the invoked object";
	BlkNum i;
	Blk *blkI;
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	allocate(a2->len);
	const Blk *a2BlkI, *b2BlkI;
	for (i = 0, blkI = blk, a2BlkI = a2->blk, b2BlkI = b2->blk;
	i < b2->len; i++, blkI++, a2BlkI++, b2BlkI++)
    *blkI = *a2BlkI | *b2BlkI;
	for (; i < a2->len; i++, blkI++, a2BlkI++)
		*blkI = *a2BlkI;
	len = a2->len;
}

// Bitwise xor
void BigUnsigned::bitXor(const BigUnsigned &a, const BigUnsigned &b) {
	// Block unsafe calls
	if (this == &a || this == &b)
		throw "BigUnsigned::bitXor: One of the arguments is the invoked object";
	BlkNum i;
	Blk *blkI;
	const BigUnsigned *a2, *b2;
	if (a.len >= b.len) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	allocate(b2->len);
	const Blk *a2BlkI, *b2BlkI;
	for (i = 0, blkI = blk, a2BlkI = a2->blk, b2BlkI = b2->blk;
	i < b2->len; i++, blkI++, a2BlkI++, b2BlkI++)
    *blkI = *a2BlkI ^ *b2BlkI;
	for (; i < a2->len; i++, blkI++, a2BlkI++)
		*blkI = *a2BlkI;
	len = a2->len;
	zapLeadingZeros();
}

// INCREMENT/DECREMENT OPERATORS

// Prefix increment
void BigUnsigned::operator ++() {
	BlkNum i;
	Blk *blkI;
	bool carry = true;
	for (i = 0, blkI = blk; i < len && carry; i++) {
		(*blkI)++;
		carry = (*blkI == 0);
	}
	if (carry) {
		allocateAndCopy(len + 1);
		*blkI = 1;
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
	BlkNum i;
	Blk *blkI;
	bool borrow = true;
	for (i = 0, blkI = blk; borrow; i++) {
		borrow = (*blkI == 0);
		(*blkI)--;
	}
	if (blk[len - 1] == 0)
		len--;
}

// Postfix decrement: same as prefix
void BigUnsigned::operator --(int) {
	operator --();
}

