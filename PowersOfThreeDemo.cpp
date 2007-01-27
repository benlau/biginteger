#include <string>
#include <iostream>
#include "BigUnsigned.h"
#include "BigInteger.h"
#include "BigIntegerIO.h"

int main() {
	std::cout << "POWERS OF THREE DEMO" << std::endl;
	std::cout << "Uses Matt McCutchen's Big Integer Library" << std::endl;
	
	BigUnsigned x(1), three(3);
	for (int power = 0; power <= 500; power++) {
		std::cout << "3^" << power << " = " << x << std::endl;
		x *= three;
	}
	
	return 0;
}
