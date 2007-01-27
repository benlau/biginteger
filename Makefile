#
# Matt McCutchen's Big Integer Library
#
# Please see the project Web site at
#    http://mysite.verizon.net/mccutchen/bigint/
# for more information and the latest version.
#
# December 16, 2004 version
#

library        : BigUnsigned.o BigInteger.o BigIntegerIO.o

BigUnsigned.o  : BigUnsigned.h BigUnsigned.cpp
	g++ -c BigUnsigned.cpp

BigInteger.o   : BigUnsigned.h BigInteger.h BigInteger.cpp
	g++ -c BigInteger.cpp

BigIntegerIO.o : BigUnsigned.h BigInteger.h BigIntegerIO.cpp
	g++ -c BigIntegerIO.cpp

po3demo        : library PowersOfThreeDemo.cpp
	g++ -opo3demo BigUnsigned.o BigInteger.o BigIntegerIO.o PowersOfThreeDemo.cpp

clean          :
	rm -f *.o po3demo
