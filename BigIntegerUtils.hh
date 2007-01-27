/*
* Matt McCutchen's Big Integer Library
* http://mysite.verizon.net/mccutchen/bigint/
*/

#ifndef BIGINTEGERUTILS
#define BIGINTEGERUTILS

#include "BigInteger.hh"
#include <string>
#include <iostream>

/*
* This file includes:
* (1) `std::string <=> BigUnsigned/BigInteger' conversion routines easier than `BigUnsignedInABase'
* (2) << and >> operators for BigUnsigned/BigInteger, std::istream/std::ostream
*/

// Conversion routines.  Base 10 only.
std::string easyBUtoString(const BigUnsigned &x);
std::string easyBItoString(const BigInteger &x);
BigUnsigned easyStringToBU(const std::string &s);
BigInteger easyStringToBI(const std::string &s);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
std::ostream &operator <<(std::ostream &os, const BigUnsigned &x);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
// My somewhat arbitrary policy: a negative sign comes before a base indicator (like -0xFF).
std::ostream &operator <<(std::ostream &os, const BigInteger &x);

#endif
