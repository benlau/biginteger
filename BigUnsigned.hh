#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H

#include "NumberlikeArray.hh"

/* A BigUnsigned object represents a nonnegative integer of size limited only by
 * available memory.  BigUnsigneds support most mathematical operators and can
 * be converted to and from most primitive integer types.
 *
 * The number is stored as a NumberlikeArray of unsigned longs as if it were
 * written in base 256^sizeof(unsigned long).  The least significant block is
 * first, and the length is such that the most significant block is nonzero. */
class BigUnsigned : protected NumberlikeArray<unsigned long> {

public:
	// Enumeration for the result of a comparison.
	enum CmpRes { less = -1, equal = 0, greater = 1 };

	// BigUnsigneds are built with a Blk type of unsigned long.
	typedef unsigned long Blk;

	typedef NumberlikeArray<Blk>::Index Index;
	NumberlikeArray<Blk>::N;

protected:
	// Creates a BigUnsigned with a capacity; for internal use.
	BigUnsigned(int, Index c) : NumberlikeArray<Blk>(0, c) {}

	// Decreases len to eliminate any leading zero blocks.
	void zapLeadingZeros() { 
		while (len > 0 && blk[len - 1] == 0)
			len--;
	}

public:
	// Constructs zero.
	BigUnsigned() : NumberlikeArray<Blk>() {}

	// Copy constructor
	BigUnsigned(const BigUnsigned &x) : NumberlikeArray<Blk>(x) {}

	// Assignment operator
	void operator=(const BigUnsigned &x) {
		NumberlikeArray<Blk>::operator =(x);
	}

	// Constructor that copies from a given array of blocks.
	BigUnsigned(const Blk *b, Index blen) : NumberlikeArray<Blk>(b, blen) {
		// Eliminate any leading zeros we may have been passed.
		zapLeadingZeros();
	}

	// Destructor.  NumberlikeArray does the delete for us.
	~BigUnsigned() {}
	
	// Constructors from primitive integer types
	BigUnsigned(unsigned long  x);
	BigUnsigned(         long  x);
	BigUnsigned(unsigned int   x);
	BigUnsigned(         int   x);
	BigUnsigned(unsigned short x);
	BigUnsigned(         short x);
protected:
	// Helpers
	template <class X> void initFromPrimitive(X x);
	template <class X> void initFromSignedPrimitive(X x);
public:

	/* Converters to primitive integer types
	 * The implicit conversion operators caused trouble, so these are now
	 * named. */
	unsigned long  toUnsignedLong () const;
	long           toLong         () const;
	unsigned int   toUnsignedInt  () const;
	int            toInt          () const;
	unsigned short toUnsignedShort() const;
	short          toShort        () const;
protected:
	// Helpers
	template <class X> X convertToSignedPrimitive() const;
	template <class X> X convertToPrimitive() const;
public:

	// ACCESSORS

	// Expose these from NumberlikeArray directly.
	NumberlikeArray<Blk>::getCapacity;
	NumberlikeArray<Blk>::getLength;

	/* Returns the requested block, or 0 if it is beyond the length (as if
	 * the number had 0s infinitely to the left). */
	Blk getBlock(Index i) const { return i >= len ? 0 : blk[i]; }

	// The number is zero if and only if the canonical length is zero.
	bool isZero() const { return NumberlikeArray<Blk>::isEmpty(); }

	// COMPARISONS

	// Compares this to x like Perl's <=>
	CmpRes compareTo(const BigUnsigned &x) const;

	// Ordinary comparison operators
	bool operator ==(const BigUnsigned &x) const {
		return NumberlikeArray<Blk>::operator ==(x);
	}
	bool operator !=(const BigUnsigned &x) const {
		return NumberlikeArray<Blk>::operator !=(x);
	}
	bool operator < (const BigUnsigned &x) const { return compareTo(x) == less   ; }
	bool operator <=(const BigUnsigned &x) const { return compareTo(x) != greater; }
	bool operator >=(const BigUnsigned &x) const { return compareTo(x) != less   ; }
	bool operator > (const BigUnsigned &x) const { return compareTo(x) == greater; }

	/*
	 * BigUnsigned and BigInteger both provide three kinds of operators.
	 * Here ``big-integer'' refers to BigInteger or BigUnsigned.
	 *
	 * (1) Overloaded ``return-by-value'' operators:
	 *     +, -, *, /, %, unary -, &, |, ^, <<, >>.
	 * Big-integer code using these operators looks identical to code using
	 * the primitive integer types.  These operators take one or two
	 * big-integer inputs and return a big-integer result, which can then
	 * be assigned to a BigInteger variable or used in an expression.
	 * Example:
	 *     BigInteger a(1), b = 1;
	 *     BigInteger c = a + b;
	 *
	 * (2) Overloaded assignment operators:
	 *     +=, -=, *=, /=, %=, flipSign, &=, |=, ^=, <<=, >>=, ++, --.
	 * Again, these are used on big integers just like on ints.  They take
	 * one writable big integer that both provides an operand and receives a
	 * result.  Most also take a second read-only operand.
	 * Example:
	 *     BigInteger a(1), b(1);
	 *     a += b;
	 *
	 * (3) Copy-less operations: `add', `subtract', etc.
	 * These named methods take operands as arguments and store the result
	 * in the receiver (*this), avoiding unnecessary copies and allocations.
	 * `divideWithRemainder' is special: it both takes the dividend from and
	 * stores the remainder into the receiver, and it takes a separate
	 * object in which to store the quotient.  NOTE: If you are wondering
	 * why these don't return a value, you probably mean to use the
	 * overloaded return-by-value operators instead.
	 * 
	 * Examples:
	 *     BigInteger a(43), b(7), c, d;
	 *
	 *     c = a + b;   // Now c == 50.
	 *     c.add(a, b); // Same effect but without the two copies.
	 *
	 *     c.divideWithRemainder(b, d);
	 *     // 50 / 7; now d == 7 (quotient) and c == 1 (remainder).
	 *
	 *     // ``Aliased'' calls now do the right thing using a temporary
	 *     // copy, but see note on `divideWithRemainder'.
	 *     a.add(a, b); 
	 */

	// COPY-LESS OPERATIONS

	// These 8: Arguments are read-only operands, result is saved in *this.
	void add(const BigUnsigned &a, const BigUnsigned &b);
	void subtract(const BigUnsigned &a, const BigUnsigned &b);
	void multiply(const BigUnsigned &a, const BigUnsigned &b);
	void bitAnd(const BigUnsigned &a, const BigUnsigned &b);
	void bitOr(const BigUnsigned &a, const BigUnsigned &b);
	void bitXor(const BigUnsigned &a, const BigUnsigned &b);
	/* Negative shift amounts translate to opposite-direction shifts,
	 * except for -2^(8*sizeof(int)-1) which is unimplemented. */
	void bitShiftLeft(const BigUnsigned &a, int b);
	void bitShiftRight(const BigUnsigned &a, int b);

	/* `a.divideWithRemainder(b, q)' is like `q = a / b, a %= b'.
	 * / and % use semantics similar to Knuth's, which differ from the
	 * primitive integer semantics under division by zero.  See the
	 * implementation in BigUnsigned.cc for details.
	 * `a.divideWithRemainder(b, a)' throws an exception: it doesn't make
	 * sense to write quotient and remainder into the same variable. */
	void divideWithRemainder(const BigUnsigned &b, BigUnsigned &q);

	/* `divide' and `modulo' are no longer offered.  Use
	 * `divideWithRemainder' instead. */

	// OVERLOADED RETURN-BY-VALUE OPERATORS
	BigUnsigned operator +(const BigUnsigned &x) const;
	BigUnsigned operator -(const BigUnsigned &x) const;
	BigUnsigned operator *(const BigUnsigned &x) const;
	BigUnsigned operator /(const BigUnsigned &x) const;
	BigUnsigned operator %(const BigUnsigned &x) const;
	/* OK, maybe unary minus could succeed in one case, but it really
	 * shouldn't be used, so it isn't provided. */
	BigUnsigned operator &(const BigUnsigned &x) const;
	BigUnsigned operator |(const BigUnsigned &x) const;
	BigUnsigned operator ^(const BigUnsigned &x) const;
	BigUnsigned operator <<(int b) const;
	BigUnsigned operator >>(int b) const;

	// OVERLOADED ASSIGNMENT OPERATORS
	void operator +=(const BigUnsigned &x);
	void operator -=(const BigUnsigned &x);
	void operator *=(const BigUnsigned &x);
	void operator /=(const BigUnsigned &x);
	void operator %=(const BigUnsigned &x);
	void operator &=(const BigUnsigned &x);
	void operator |=(const BigUnsigned &x);
	void operator ^=(const BigUnsigned &x);
	void operator <<=(int b);
	void operator >>=(int b);

	/* INCREMENT/DECREMENT OPERATORS
	 * To discourage messy coding, these do not return *this, so prefix
	 * and postfix behave the same. */
	void operator ++(   );
	void operator ++(int);
	void operator --(   );
	void operator --(int);

	// Helper function that needs access to BigUnsigned internals
	friend Blk getShiftedBlock(const BigUnsigned &num, Index x,
			unsigned int y);

	// See BigInteger.cc.
	template <class X>
	friend X convertBigUnsignedToPrimitiveAccess(const BigUnsigned &a);
};

/* Implementing the return-by-value and assignment operators in terms of the
 * copy-less operations.  The copy-less operations are responsible for making
 * any necessary temporary copies to work around aliasing. */

inline BigUnsigned BigUnsigned::operator +(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.add(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator -(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.subtract(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator *(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.multiply(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator /(const BigUnsigned &x) const {
	if (x.isZero()) throw "BigUnsigned::operator /: division by zero";
	BigUnsigned q, r;
	r = *this;
	r.divideWithRemainder(x, q);
	return q;
}
inline BigUnsigned BigUnsigned::operator %(const BigUnsigned &x) const {
	if (x.isZero()) throw "BigUnsigned::operator %: division by zero";
	BigUnsigned q, r;
	r = *this;
	r.divideWithRemainder(x, q);
	return r;
}
inline BigUnsigned BigUnsigned::operator &(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.bitAnd(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator |(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.bitOr(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator ^(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.bitXor(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator <<(int b) const {
	BigUnsigned ans;
	ans.bitShiftLeft(*this, b);
	return ans;
}
inline BigUnsigned BigUnsigned::operator >>(int b) const {
	BigUnsigned ans;
	ans.bitShiftRight(*this, b);
	return ans;
}

inline void BigUnsigned::operator +=(const BigUnsigned &x) {
	add(*this, x);
}
inline void BigUnsigned::operator -=(const BigUnsigned &x) {
	subtract(*this, x);
}
inline void BigUnsigned::operator *=(const BigUnsigned &x) {
	multiply(*this, x);
}
inline void BigUnsigned::operator /=(const BigUnsigned &x) {
	if (x.isZero()) throw "BigUnsigned::operator /=: division by zero";
	/* The following technique is slightly faster than copying *this first
	 * when x is large. */
	BigUnsigned q;
	divideWithRemainder(x, q);
	// *this contains the remainder, but we overwrite it with the quotient.
	*this = q;
}
inline void BigUnsigned::operator %=(const BigUnsigned &x) {
	if (x.isZero()) throw "BigUnsigned::operator %=: division by zero";
	BigUnsigned q;
	// Mods *this by x.  Don't care about quotient left in q.
	divideWithRemainder(x, q);
}
inline void BigUnsigned::operator &=(const BigUnsigned &x) {
	bitAnd(*this, x);
}
inline void BigUnsigned::operator |=(const BigUnsigned &x) {
	bitOr(*this, x);
}
inline void BigUnsigned::operator ^=(const BigUnsigned &x) {
	bitXor(*this, x);
}
inline void BigUnsigned::operator <<=(int b) {
	bitShiftLeft(*this, b);
}
inline void BigUnsigned::operator >>=(int b) {
	bitShiftRight(*this, b);
}

#endif
