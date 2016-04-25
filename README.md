QML BigInteger Library
======================

It is a library to provide BigInteger for QML.
It is derived from the work by [Matt McCutchen](https://mattmccutchen.net/bigint/), and changed the licesne to APACHE-2.0.

Only C++11 support has been added to the original work. 
In case you only need to use BigInteger in Qt/C++ application, 
you may just include all the *.cc files from this project.

Installation
============

	qpm install com.github.benlau.biginteger

API
===

```
import BigInteger 1.0
```

string BigInteger.add(string a, string b)

string BigInteger.multiply(string a, string b)
string BigInteger.multiply(string a, stringlist fraction)

string BigInteger.minus(string a, string b)

string BigInteger.divide(string a, string b)

string BigInteger.abs(string value)

bool BigInteger.isNegative(string value)

int BigInteger.compare(string a, string b)

bool BigInteger.greater(string a, string b)

bool BigInteger.greaterOrEquals(string a, string b)

bool BigInteger.lesser(string a, string b)

bool BigInteger.lesserOrEquals(string a, string b)

bool BigInteger.equals(string a, string b)

array fraction(qreal input)

string gcd(string a, string b)

string pow(string a, int b)


