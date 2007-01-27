/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#ifndef BIGINTEGER
#define BIGINTEGER

#include "BigUnsigned.h"

/*
*
* A BigInteger object represents a signed integer of size
* limited only by available memory.  A BigInteger can be
* created from and converted back to most integral types,
* and many math operations are defined on them.
*
* The number is stored as a series of blocks in a
* dynamically allocated array.  It is as if the numbers
* were written digit by digit in base 2^32.
*
* This class is derived from BigUnsigned, which represents
* a large nonnegative integer.  This should be understood
* first, as only new or different things are declared here.
* Some things are redeclared so that they use the BigInteger
* versions of methods, rather than the BigUnsigned versions.
*/

class BigInteger : public BigUnsigned {
	
	// TYPES & CONSTANTS
	public:
	enum Sign { negative = -1, zero = 0, positive = 1 }; // Enumeration for the sign of a BigInteger
	
	// FIELDS
	protected:
	Sign sign; // The sign of this BigInteger
	
	// MANAGEMENT
	protected:
	BigInteger(Sign s, BlkNum c); // Creates a BigInteger with a sign and capacity
	public:
	BigInteger(); // Default constructor (value is 0)
	BigInteger(const BigInteger &x); // Copy constructor
	void operator=(const BigInteger &x); // Assignment operator
	BigInteger(const Blk *b, BlkNum l); // Constructor from an array of blocks
	BigInteger(const Blk *b, BlkNum l, Sign s); // Constructor from an array of blocks and a sign
	BigInteger(const BigUnsigned &x); // Constructor from a BigUnsigned
	BigInteger(const BigUnsigned &x, Sign s); // Constructor from a BigUnsigned and a sign
	// Constructors from integral types
	BigInteger(unsigned long  x);
	BigInteger(         long  x);
	BigInteger(unsigned int   x);
	BigInteger(         int   x);
	BigInteger(unsigned short x);
	BigInteger(         short x);
	// Note that a BigInteger can be converted to a BigUnsigned
	// automatically; this takes its absolute value.
	
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
	Sign getSign() const;
	
	// COMPARISONS
	public:
	// Compares this to x like Perl's <=>
	CmpRes compareTo(const BigInteger &x) const;
	// Normal comparison operators
	bool operator ==(const BigInteger &x) const;
	bool operator !=(const BigInteger &x) const;
	bool operator < (const BigInteger &x) const;
	bool operator <=(const BigInteger &x) const;
	bool operator >=(const BigInteger &x) const;
	bool operator > (const BigInteger &x) const;
	
	// PUT-HERE OPERATIONS
	/* These store the result of the operation on the arguments into this.
	* a.add(b, c) is equivalent to, but faster than, a = b + c.
	* Calls like a.operation(a, b) are unsafe and not allowed. */
	public:
	void add     (const BigInteger &a, const BigInteger &b); // Addition
	void subtract(const BigInteger &a, const BigInteger &b); // Subtraction
	void multiply(const BigInteger &a, const BigInteger &b); // Multiplication
	void divide  (const BigInteger &a, const BigInteger &b); // Division
	void modulo  (const BigInteger &a, const BigInteger &b); // Modular reduction
	void negate  (const BigInteger &a                     ); // Negative
	// Some operations are inherently unsigned and are not
	// redefined for BigIntegers.  Calling one of these on
	// a BigInteger will take its absolute value first.
	
	// NORMAL OPERATORS
	// These perform the operation on this (to the left of the operator)
	// and x (to the right of the operator) and return a new BigInteger with the result.
	public:
	BigInteger operator +(const BigInteger &x) const; // Addition
	BigInteger operator -(const BigInteger &x) const; // Subtraction
	BigInteger operator *(const BigInteger &x) const; // Multiplication
	BigInteger operator /(const BigInteger &x) const; // Division
	BigInteger operator %(const BigInteger &x) const; // Modular reduction
	BigInteger operator -(                   ) const; // Negative
	
	// ASSIGNMENT OPERATORS
	// These perform the operation on this and x, storing the result into this.
	public:
	void operator +=(const BigInteger &x); // Addition
	void operator -=(const BigInteger &x); // Subtraction
	void operator *=(const BigInteger &x); // Multiplication
	void operator /=(const BigInteger &x); // Division
	void operator %=(const BigInteger &x); // Modular reduction
	void flipSign();                       // Negative
	
	// INCREMENT/DECREMENT OPERATORS
	// These increase or decrease the number by 1.  To discourage side effects,
	// these do not return *this, so prefix and postfix behave the same.
	public:
	void operator ++(   ); // Prefix  increment
	void operator ++(int); // Postfix decrement
	void operator --(   ); // Prefix  increment
	void operator --(int); // Postfix decrement
	
};

// MANAGEMENT
inline BigInteger::BigInteger(Sign s, BlkNum c) : BigUnsigned(0, c), sign(s) { }
inline BigInteger::BigInteger() : BigUnsigned(), sign(zero) { }
inline BigInteger::BigInteger(const BigInteger &x) : BigUnsigned(x), sign(x.sign) { }
inline BigInteger::BigInteger(const Blk *b, BlkNum l) : BigUnsigned(b, l) { sign = (len == 0) ? zero : positive; }
inline BigInteger::BigInteger(const BigUnsigned &x) : BigUnsigned(x) { sign = (len == 0) ? zero : positive; }

// PICKING APART
inline BigInteger::Sign BigInteger::getSign() const { return sign; }

// COMPARISONS
inline bool BigInteger::operator==(const BigInteger &x) const { return compareTo(x) == equal  ; }
inline bool BigInteger::operator!=(const BigInteger &x) const { return compareTo(x) != equal  ; }
inline bool BigInteger::operator< (const BigInteger &x) const { return compareTo(x) == less   ; }
inline bool BigInteger::operator<=(const BigInteger &x) const { return compareTo(x) != greater; }
inline bool BigInteger::operator>=(const BigInteger &x) const { return compareTo(x) != less   ; }
inline bool BigInteger::operator> (const BigInteger &x) const { return compareTo(x) == greater; }

// NORMAL OPERATORS
/* These create an object to hold the result and invoke
* the appropriate put-here operation on it, passing
* this and x.  The new object is then returned. */
inline BigInteger BigInteger::operator +(const BigInteger &x) const {
	BigInteger ans;
	ans.add(*this, x);
	return ans;
}
inline BigInteger BigInteger::operator -(const BigInteger &x) const {
	BigInteger ans;
	ans.subtract(*this, x);
	return ans;
}
inline BigInteger BigInteger::operator *(const BigInteger &x) const {
	BigInteger ans;
	ans.multiply(*this, x);
	return ans;
}
inline BigInteger BigInteger::operator /(const BigInteger &x) const {
	BigInteger ans;
	ans.divide(*this, x);
	return ans;
}
inline BigInteger BigInteger::operator %(const BigInteger &x) const {
	BigInteger ans;
	ans.modulo(*this, x);
	return ans;
}
inline BigInteger BigInteger::operator -() const {
	BigInteger ans;
	ans.negate(*this);
	return ans;
}

// ASSIGNMENT OPERATORS
// These create a copy of this, then invoke the appropriate
// put-here operation on this, passing the copy and x.
inline void BigInteger::operator +=(const BigInteger &x) {
	BigInteger thisCopy(*this);
	add(thisCopy, x);
}
inline void BigInteger::operator -=(const BigInteger &x) {
	BigInteger thisCopy(*this);
	subtract(thisCopy, x);
}
inline void BigInteger::operator *=(const BigInteger &x) {
	BigInteger thisCopy(*this);
	multiply(thisCopy, x);
}
inline void BigInteger::operator /=(const BigInteger &x) {
	BigInteger thisCopy(*this);
	divide(thisCopy, x);
}
inline void BigInteger::operator %=(const BigInteger &x) {
	BigInteger thisCopy(*this);
	modulo(thisCopy, x);
}
// This one is trivial
inline void BigInteger::flipSign() {
	sign = Sign(-sign);
}

#endif
