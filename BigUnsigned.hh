/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#ifndef BIGUNSIGNED
#define BIGUNSIGNED

#include "NumberlikeArray.hh"

/*
* A BigUnsigned object represents a nonnegative integer of size
* limited only by available memory.  A BigUnsigned can be
* created from and converted back to most integral types,
* and many math operations are defined on BigUnsigneds.
*
* The number is stored as a series of blocks in a
* dynamically allocated array.  It is as if the number
* were written digit by digit in base 2 ^ N, **where N is the
* number of bits in an unsigned long.**
*
* The memory-management details that used to be in here have
* been moved into NumberlikeArray, which BigUnsigned now derives from.
* `(NlA)' means that member(s) are declared identically in NumberlikeArray.
* Such members are either redeclared here to make them public or are
* here, commented out, for reference.
*/

class BigUnsigned : protected NumberlikeArray<unsigned long> {
	
	// TYPES & CONSTANTS
	public:
	enum CmpRes { less = -1, equal = 0, greater = 1 }; // Enumeration for the result of a comparison
	typedef unsigned long Blk; // The number block type that BigUnsigneds are built from
	typedef NumberlikeArray<Blk>::Index Index; // (NlA) Type for the index of a block in the array
	NumberlikeArray<Blk>::N; // Number of bits in a Blk
	
	/*
	// FIELDS
	protected:
	Index cap; // (NlA) The current allocated capacity of this BigUnsigned (in blocks)
	Index len; // (NlA) The actual length of the number stored in this BigUnsigned (in blocks)
	Blk *blk; // (NlA) Dynamically allocated array of the number blocks
	*/
	
	// MANAGEMENT
	protected:
	// These members generally defer to those in NumberlikeArray, possibly with slight changes.
	// It might be nice if one could request that constructors be inherited in C++.
	
	BigUnsigned(int, Index c) : NumberlikeArray<Blk>(0, c) {} // Creates a BigUnsigned with a capacity
	
	void zapLeadingZeros() { // Decreases len to eliminate leading zeros
		while (len > 0 && blk[len - 1] == 0)
			len--;
	}
	
	//void allocate(Index c); // (NlA) Ensures the number array has at least the indicated capacity, maybe discarding contents
	//void allocateAndCopy(Index c); // (NlA) Ensures the number array has at least the indicated capacity, preserving its contents
	
	public:
	BigUnsigned() : NumberlikeArray<Blk>() {} // Default constructor (value is 0)
	BigUnsigned(const BigUnsigned &x) : NumberlikeArray<Blk>(x) {} // Copy constructor
	
	void operator=(const BigUnsigned &x) { // Assignment operator
		NumberlikeArray<Blk>::operator =(x);
	}
	
	BigUnsigned(const Blk *b, Index l) : NumberlikeArray<Blk>(b, l) { // Constructor from an array of blocks
		zapLeadingZeros();
	}
	
	// Constructors from integral types
	BigUnsigned(unsigned long  x);
	BigUnsigned(         long  x);
	BigUnsigned(unsigned int   x);
	BigUnsigned(         int   x);
	BigUnsigned(unsigned short x);
	BigUnsigned(         short x);
	~BigUnsigned() {} // Destructor
	
	// CONVERTERS to integral types
	public:
	operator unsigned long () const;
	operator          long () const;
	operator unsigned int  () const;
	operator          int  () const;
	operator unsigned short() const;
	operator          short() const;
	
	// PICKING APART
	// These accessors can be used to get the pieces of the number
	public:
	NumberlikeArray<Blk>::getCapacity;
	NumberlikeArray<Blk>::getLength;
	// Note that getBlock returns 0 if the block index is beyond the length of the number.
	// A routine that uses this accessor can safely assume a BigUnsigned has 0s infinitely to the left.
	Blk getBlock(Index i) const { return i >= len ? 0 : blk[i]; }
	// Note how we replace one level of abstraction with another.  Isn't that neat?
	bool isZero() const { return NumberlikeArray<Blk>::isEmpty(); } // Often convenient for loops
	
	// COMPARISONS
	public:
	// Compares this to x like Perl's <=>
	CmpRes compareTo(const BigUnsigned &x) const;
	// Normal comparison operators
	NumberlikeArray<Blk>::operator ==; // (NlA) The body used to be `{ return compareTo(x) == equal; }'.  For performance reasons we use NumberlikeArray code that only worries about (in)equality and doesn't waste time determining which is bigger
	NumberlikeArray<Blk>::operator !=; // (NlA) Ditto.
	bool operator < (const BigUnsigned &x) const { return compareTo(x) == less   ; }
	bool operator <=(const BigUnsigned &x) const { return compareTo(x) != greater; }
	bool operator >=(const BigUnsigned &x) const { return compareTo(x) != less   ; }
	bool operator > (const BigUnsigned &x) const { return compareTo(x) == greater; }

	/*
	* BigUnsigned and BigInteger both provide three kinds of operators.
	* Here ``big-integer'' refers to BigInteger or BigUnsigned.
	*
	* (1) Overloaded ``return-by-value'' operators:
	*     +, -, *, /, %, unary -.
	* Big-integer code using these operators looks identical to
	* code using the primitive integer types.  These operators take
	* one or two big-integer inputs and return a big-integer result,
	* which can then be assigned to a BigInteger variable or used
	* in an expression.  Example:
	*     BigInteger a(1), b = 1;
	*     BigInteger c = a + b;
	*
	* (2) Overloaded assignment operators:
	*     +=, -=, *=, /=, %=, &=, |=, ^=, ++, --, flipSign.
	* Again, these are used on big integers just like on ints.
	* They take one writable big integer that both provides an
	* operand and receives a result.  The first eight also take
	* a second read-only operand.  Example:
	*     BigInteger a(1), b(1);
	*     a += b;
	*
	* (3) ``Put-here'' operations: `add', `subtract', etc.
	* Using a return-by-value or assignment operator generally involves
	* copy constructions and/or assignments.  The ``put-here'' operations
	* require none, but they are more of a hassle to use.  Most take two
	* read-only operands and save the result in the calling object `*this',
	* whose previous value is ignored.  `divideWithRemainder' is an exception.
	* <<< NOTE >>>: Put-here operations do not return a value: they don't need to!!
	* Examples:
	*     BigInteger a(43), b(7), c, d;
	*     c = a + b;   // Now c == 50.
	*     c.add(a, b); // Same effect but without the two bulk-copies.
	*     c.divideWithRemainder(b, d); // 50 / 7; now d == 7 (quotient) and c == 1 (remainder).
	*     a.add(a, b); // Unsafe ``aliased'' call; causes a runtime error.
	*/
	
	// PUT-HERE OPERATIONS
	public:
	/* These 3: Two read-only operands as arguments.  Result left in *this. */
	void add(const BigUnsigned &a, const BigUnsigned &b); // Addition
	void subtract(const BigUnsigned &a, const BigUnsigned &b); // Subtraction
	void multiply(const BigUnsigned &a, const BigUnsigned &b); // Multiplication
	/* Divisive stuff
	* `a.divideWithRemainder(b, q)' is like `q = a / b, a %= b'.
	* Semantics similar to Donald E. Knuth's are used for / and %,
	* and these differ from the semantics of primitive-type
	* / and % under division by zero.
	* Look in `BigUnsigned.cc' for details.
	*/
	void divideWithRemainder(const BigUnsigned &b, BigUnsigned &q);
	void divide(const BigUnsigned &a, const BigUnsigned &b) {
		// Division, deprecated and provided for backward compatibility
		BigUnsigned a2(a);
		a2.divideWithRemainder(b, *this);
		// quotient now in *this
		// don't care about remainder left in a2
	}
	void modulo(const BigUnsigned &a, const BigUnsigned &b) {
		// Modular reduction, deprecated and provided for backward compatibility
		*this = a;
		BigUnsigned q;
		divideWithRemainder(b, q);
		// remainder now in *this
		// don't care about quotient left in q
	}
	// Bitwise operations.  Two read-only operands as arguments.  Result left in *this.
	// These are not provided for BigIntegers; I think that using them on BigIntegers
	// will discard the sign first.
	void bitAnd(const BigUnsigned &a, const BigUnsigned &b); // Bitwise AND
	void bitOr(const BigUnsigned &a, const BigUnsigned &b); // Bitwise OR
	void bitXor(const BigUnsigned &a, const BigUnsigned &b); // Bitwise XOR
	
	// These functions are declared but not defined.  (Sorry.)
	// Trying to call either will result in a link-time error.
	void bitShiftLeft(const BigUnsigned &a, unsigned int b); // Bitwise left shift
	void bitShiftRight(const BigUnsigned &a, unsigned int b); // Bitwise right shift
	
	// NORMAL OPERATORS
	// These perform the operation on this (to the left of the operator)
	// and x (to the right of the operator) and return a new BigUnsigned with the result.
	public:
	BigUnsigned operator +(const BigUnsigned &x) const; // Addition
	BigUnsigned operator -(const BigUnsigned &x) const; // Subtraction
	BigUnsigned operator *(const BigUnsigned &x) const; // Multiplication
	BigUnsigned operator /(const BigUnsigned &x) const; // Division
	BigUnsigned operator %(const BigUnsigned &x) const; // Modular reduction
	BigUnsigned operator &(const BigUnsigned &x) const; // Bitwise AND
	BigUnsigned operator |(const BigUnsigned &x) const; // Bitwise OR
	BigUnsigned operator ^(const BigUnsigned &x) const; // Bitwise XOR
	
	// ASSIGNMENT OPERATORS
	// These perform the operation on this and x, storing the result into this.
	public:
	void operator +=(const BigUnsigned &x); // Addition
	void operator -=(const BigUnsigned &x); // Subtraction
	void operator *=(const BigUnsigned &x); // Multiplication
	void operator /=(const BigUnsigned &x); // Division
	void operator %=(const BigUnsigned &x); // Modular reduction
	void operator &=(const BigUnsigned &x); // Bitwise AND
	void operator |=(const BigUnsigned &x); // Bitwise OR
	void operator ^=(const BigUnsigned &x); // Bitwise XOR
	
	// INCREMENT/DECREMENT OPERATORS
	// These increase or decrease the number by 1.  To discourage side effects,
	// these do not return *this, so prefix and postfix behave the same.
	public:
	void operator ++(   ); // Prefix  increment
	void operator ++(int); // Postfix decrement
	void operator --(   ); // Prefix  increment
	void operator --(int); // Postfix decrement
	
	// Helper function that needs access to BigUnsigned internals
	friend Blk getShiftedBlock(const BigUnsigned &num, Index x, unsigned int y);
};

// NORMAL OPERATORS
/* These create an object to hold the result and invoke
* the appropriate put-here operation on it, passing
* this and x.  The new object is then returned. */
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
	BigUnsigned ans;
	ans.divide(*this, x);
	return ans;
}
inline BigUnsigned BigUnsigned::operator %(const BigUnsigned &x) const {
	BigUnsigned ans;
	ans.modulo(*this, x);
	return ans;
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

// ASSIGNMENT OPERATORS
// These create a copy of this, then invoke the appropriate
// put-here operation on this, passing the copy and x.
// Exception: those updated for divideWithRemainder.
inline void BigUnsigned::operator +=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	add(thisCopy, x);
}
inline void BigUnsigned::operator -=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	subtract(thisCopy, x);
}
inline void BigUnsigned::operator *=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	multiply(thisCopy, x);
}
inline void BigUnsigned::operator /=(const BigUnsigned &x) {
	// Updated for divideWithRemainder
	BigUnsigned thisCopy(*this);
	thisCopy.divideWithRemainder(x, *this);
	// quotient left in *this
	// don't care about remainder left in thisCopy
}
inline void BigUnsigned::operator %=(const BigUnsigned &x) {
	// Shortcut (woohoo!)
	BigUnsigned q;
	divideWithRemainder(x, q);
	// remainder left in *this
	// don't care about quotient left in q
}
inline void BigUnsigned::operator &=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	bitAnd(thisCopy, x);
}
inline void BigUnsigned::operator |=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	bitOr(thisCopy, x);
}
inline void BigUnsigned::operator ^=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	bitXor(thisCopy, x);
}

#endif
