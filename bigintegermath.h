#ifndef BIGINTEGERMATH_H
#define BIGINTEGERMATH_H

#include "BigInteger.hh"

/// Math utility

class BigIntegerMath
{
public:

    static BigInteger pow(const BigInteger &base,int exponent);

    static BigInteger gcd(const BigInteger &a,const BigInteger &b);
};

#endif // BIGINTEGERMATH_H
