/*
* Matt McCutchen's Big Integer Library
* See: http://mysite.verizon.net/mccutchen/bigint/
*/

#ifndef BIGINTEGERIO
#define BIGINTEGERIO

#include "BigUnsigned.h"
#include "BigInteger.h"
#include <iostream>

// Some I/O routines for BigIntegers and BigUnsigneds

// Both of these output x to os in decimal notation.
std::ostream &operator <<(std::ostream &os, BigUnsigned x);
std::ostream &operator <<(std::ostream &os, BigInteger x);

#endif
