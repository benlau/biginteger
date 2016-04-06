import QtQuick 2.0
import QtTest 1.0
import BigInteger 1.0

TestCase {
    name: "CalculationTests"

    function test_multiply() {
        compare(BigInteger.multiply("3141592653589793238462643383279","3141592653589793238462643383279"), "9869604401089358618834490999872991420714831834895839696791841");
    }

    function test_divide() {
        compare(BigInteger.divide("1231233141592653589793238462643383279","9128391823091"), "134879523738031326302006");
    }

    function test_isNegative() {
        compare(BigInteger.isNegative("1231233141592653589793238462643383279"), false);
        compare(BigInteger.isNegative("-1231233141592653589793238462643383279"), true);
    }

    function test_compare() {
        compare(BigInteger.compare("81723121231231233","123123123") , 1);
        compare(BigInteger.greater("81723121231231233","123123123"), true);
        compare(BigInteger.greaterOrEquals("81723121231231233","123123123"), true);

        compare(BigInteger.compare("123123123","81723121231231233") , -1);
        compare(BigInteger.lesser("123123123","81723121231231233"), true);
        compare(BigInteger.lesserOrEquals("123123123","81723121231231233"), true);

        compare(BigInteger.compare("81723121231231233","81723121231231233") , 0);
        compare(BigInteger.equals("81723121231231233","81723121231231233") , true);
        compare(BigInteger.lesserOrEquals("81723121231231233","81723121231231233") , true);
        compare(BigInteger.greaterOrEquals("81723121231231233","81723121231231233") , true);
    }

    function test_abs() {
        compare(BigInteger.abs("-12312312381723121231231233") , "12312312381723121231231233");
        compare(BigInteger.abs("12312312381723121231231233") , "12312312381723121231231233");
    }

}
