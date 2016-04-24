#include "bigintegermath.h"


BigInteger BigIntegerMath::pow(const BigInteger &base, int exponent)
{
    BigInteger result;

    if (exponent <= 1) {
        // It don't support floating point
        result = 1;
        return result;
    }

    BigInteger x = base;
    BigInteger y = 1;
    int n = exponent;

    while (n > 1) {
        if ((n & 1) == 0) { // even number
            x = x * x;
            n = n / 2;
        } else {
            y = x * y;
            x = x * x;
            n = ( n - 1) / 2;
        }
    }

    result = x * y;
    return result;
}
