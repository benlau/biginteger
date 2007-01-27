/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#ifndef BIGUNSIGNED
#define BIGUNSIGNED

/*
* A BigUnsigned object represents a nonnegative integer of size
* limited only by available memory.  A BigUnsigned can be
* created from and converted back to most integral types,
* and many math operations are defined on them.
*
* The number is stored as a series of blocks in a
* dynamically allocated array.  It is as if the numbers
* were written digit by digit in base 2^32.
*/

class BigUnsigned {
	
	// TYPES & CONSTANTS
	public:
	enum CmpRes { less = -1, equal = 0, greater = 1 }; // Enumeration for the result of a comparison
	typedef unsigned long Blk; // The number block type that BigUnsigneds are built from
	typedef unsigned int BlkNum; // Type for the index of a block in the array
	
	// FIELDS
	protected:
	BlkNum cap; // The current allocated capacity of this BigUnsigned (in blocks)
	BlkNum len; // The actual length of the number stored in this BigUnsigned (in blocks)
	Blk *blk; // Dynamically allocated array of the number blocks
	
	// MANAGEMENT
	protected:
	BigUnsigned(int, BlkNum c); // Creates a BigUnsigned with a capacity
	void zapLeadingZeros(); // Decreases len to eliminate leading zeros
	void allocate(BlkNum c); // Ensures the number array has at least the indicated capacity, maybe discarding contents
	void allocateAndCopy(BlkNum c); // Ensures the number array has at least the indicated capacity, preserving its contents
	public:
	BigUnsigned(); // Default constructor (value is 0)
	BigUnsigned(const BigUnsigned &x); // Copy constructor
	void operator=(const BigUnsigned &x); // Assignment operator
	BigUnsigned(const Blk *b, BlkNum l); // Constructor from an array of blocks
	// Constructors from integral types
	BigUnsigned(unsigned long  x);
	BigUnsigned(         long  x);
	BigUnsigned(unsigned int   x);
	BigUnsigned(         int   x);
	BigUnsigned(unsigned short x);
	BigUnsigned(         short x);
	~BigUnsigned(); // Destructor
	
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
	BlkNum getCapacity() const;
	BlkNum getLength() const;
	Blk getBlock(BlkNum i) const;
	bool isZero() const; // Often convenient for loops
	
	// COMPARISONS
	public:
	// Compares this to x like Perl's <=>
	CmpRes compareTo(const BigUnsigned &x) const;
	// Normal comparison operators
	bool operator ==(const BigUnsigned &x) const;
	bool operator !=(const BigUnsigned &x) const;
	bool operator < (const BigUnsigned &x) const;
	bool operator <=(const BigUnsigned &x) const;
	bool operator >=(const BigUnsigned &x) const;
	bool operator > (const BigUnsigned &x) const;
	
	// PUT-HERE OPERATIONS
	/* These store the result of the operation on the arguments into this.
	* a.add(b, c) is equivalent to, but faster than, a = b + c.
	* Calls like a.operation(a, b) are unsafe and not allowed. */
	public:
	void add          (const BigUnsigned &a, const BigUnsigned &b); // Addition
	void subtract     (const BigUnsigned &a, const BigUnsigned &b); // Subtraction
	void multiply     (const BigUnsigned &a, const BigUnsigned &b); // Multiplication
	void divide       (const BigUnsigned &a, const BigUnsigned &b); // Division
	void modulo       (const BigUnsigned &a, const BigUnsigned &b); // Modular reduction
	void bitAnd       (const BigUnsigned &a, const BigUnsigned &b); // Bitwise AND
	void bitOr        (const BigUnsigned &a, const BigUnsigned &b); // Bitwise OR
	void bitXor       (const BigUnsigned &a, const BigUnsigned &b); // Bitwise XOR
	void bitShiftLeft (const BigUnsigned &a, unsigned int       b); // Bitwise left shift
	void bitShiftRight(const BigUnsigned &a, const BigUnsigned &b); // Bitwise right shift
	
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
	
};

// MANAGEMENT

inline BigUnsigned::BigUnsigned(int, BlkNum c) : cap(c), len(0) {
	blk = new Blk[cap];
}

inline void BigUnsigned::zapLeadingZeros() {
	for (Blk *blkLenM1 = blk + len - 1; len > 0 && *blkLenM1 == 0; len--, blkLenM1--)
		;
}

inline BigUnsigned::BigUnsigned() : cap(0), len(0) {
	blk = new Blk[0];
}

inline BigUnsigned::~BigUnsigned() {
	delete [] blk;
}

// PICKING APART
inline BigUnsigned::BlkNum BigUnsigned::getCapacity() const { return cap; }
inline BigUnsigned::BlkNum BigUnsigned::getLength() const { return len; }
// Note that getBlock returns 0 if the block index is beyond the length of the number.
// A routine that uses this accessor can safely assume a BigUnsigned has 0s infinitely to the left.
inline BigUnsigned::Blk BigUnsigned::getBlock(BlkNum i) const { return i >= len ? 0 : blk[i]; }
inline bool BigUnsigned::isZero() const { return len == 0; }

// COMPARISONS
inline bool BigUnsigned::operator==(const BigUnsigned &x) const { return compareTo(x) == equal  ; }
inline bool BigUnsigned::operator!=(const BigUnsigned &x) const { return compareTo(x) != equal  ; }
inline bool BigUnsigned::operator< (const BigUnsigned &x) const { return compareTo(x) == less   ; }
inline bool BigUnsigned::operator<=(const BigUnsigned &x) const { return compareTo(x) != greater; }
inline bool BigUnsigned::operator>=(const BigUnsigned &x) const { return compareTo(x) != less   ; }
inline bool BigUnsigned::operator> (const BigUnsigned &x) const { return compareTo(x) == greater; }

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
	BigUnsigned thisCopy(*this);
	divide(thisCopy, x);
}
inline void BigUnsigned::operator %=(const BigUnsigned &x) {
	BigUnsigned thisCopy(*this);
	modulo(thisCopy, x);
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
