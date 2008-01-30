#include "BigIntegerAlgorithms.hh"

BigUnsigned gcd(BigUnsigned a, BigUnsigned b) {
	BigUnsigned trash;
	// Neat in-place alternating technique.
	for (;;) {
		if (b.isZero())
			return a;
		a.divideWithRemainder(b, trash);
		if (a.isZero())
			return b;
		b.divideWithRemainder(a, trash);
	}
}

void extendedEuclidean(BigInteger m, BigInteger n,
		BigInteger &g, BigInteger &r, BigInteger &s) {
	if (&g == &r || &g == &s || &r == &s)
		throw "BigInteger extendedEuclidean: Outputs are aliased";
	BigInteger r1(1), s1(0), r2(0), s2(1), q;
	/* Invariants:
	 * r1*m + s1*n == m(orig)
	 * r2*m + s2*n == n(orig) */
	for (;;) {
		if (n.isZero()) {
			r = r1; s = s1; g = m;
			return;
		}
		m.divideWithRemainder(n, q);
		r1 -= q*r2; s1 -= q*s2;

		if (m.isZero()) {
			r = r2; s = s2; g = n;
			return;
		}
		n.divideWithRemainder(m, q);
		r2 -= q*r1; s2 -= q*s1;
	}
}

BigUnsigned modinv(const BigInteger &x, const BigUnsigned &n) {
	BigInteger g, r, s;
	extendedEuclidean(x, n, g, r, s);
	if (g == 1)
		// r*x + s*n == 1, so r*x === 1 (mod n), so r is the answer.
		return (r % n).getMagnitude(); // (r % n) will be nonnegative
	else
		throw "BigInteger modinv: x and n have a common factor";
}

BigUnsigned modexp(const BigInteger &base, const BigUnsigned &exponent,
		const BigUnsigned &modulus) {
	BigUnsigned ans = 1, base2 = (base % modulus).getMagnitude();
	BigUnsigned::Index i = exponent.getLength();
	// For each block of the exponent, most to least significant...
	while (i > 0) {
		i--;
		BigUnsigned::Blk eb = exponent.getBlock(i);
		// For each bit, most to least significant...
		for (BigUnsigned::Blk mask = ~((~BigUnsigned::Blk(0)) >> 1);
				mask != 0; mask >>= 1) {
			// Square and maybe multiply.
			ans *= ans;
			ans %= modulus;
			if (eb & mask) {
				ans *= base2;
				ans %= modulus;
			}
		}
	}
	return ans;
}
