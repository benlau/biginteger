QML BigInteger Library
======================

It is a library to provide BigInteger for QML.
It is derived from the work by [Matt McCutchen](https://mattmccutchen.net/bigint/), and changed the licesne to APACHE-2.0.

The original source is almost unmodified except for C++11 support.

In case you only need to use BigInteger in Qt/C++ application, 
you may just include all the *.cc files from this project.

Installation
============

	qpm install com.github.benlau.biginteger

2.0 API
======

```
import BigInteger 2.0
```

Usage
-----

**BigInteger.create(number)**

Create a BigInteger object. You can pass in :

1. A string
2. A Javascript number
3. Undefined - The result will be "0"

**BigInteger.abs(number)**

**BigInteger.gcd(a,b)**

**BigInteger.pow(base, exp)**

**BigInteger.fraction(number)**

Methods
-------

**toString()**


**equals(number)**

**add(number)**

```
var num = BigInteger.create("1000").add(3); // 1003
```

**multiply(number)**

**divide(number)**

**minus(number)**

**isNegative()**

**compare(number)**

**greater(number)**

**greaterOrEquals(nunber)**

**lesser(number)**

**lesserOrEquals(number)**



1.0 API
===

```
import BigInteger 1.0
```

string BigInteger.add(string a, string b)

string BigInteger.multiply(string a, string b)

string BigInteger.multiply(string a, stringlist fraction)

string BigInteger.multiply(string a, real b)

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


