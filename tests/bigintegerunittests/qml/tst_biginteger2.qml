import QtQuick 2.0
import QtTest 1.0
import BigInteger 2.0

TestCase {
    name: "BigInteger2Tests"

    function test_create() {
        var a = BigInteger.create(1000);
        var b = BigInteger.create("1000");
        var c = BigInteger.create();

        compare(a.toString(), "1000");
        compare(b.toString(), "1000");
        compare(c.toString(), "0");

        compare(a.equals(b), true);
        compare(b.equals(c), false);
    }

    function test_multiply() {
        compare(BigInteger.create("3141592653589793238462643383279").multiply("3141592653589793238462643383279").equals("9869604401089358618834490999872991420714831834895839696791841"), true);
        compare(BigInteger.create("134").multiply([ "5", "7"]).equals("95"), true);
        compare(BigInteger.create("134").multiply(0.714285).equals("95"), true);
        compare(BigInteger.create("10").multiply([8,10]).equals("8"), true);
        compare(BigInteger.create("10").multiply(0.8).equals("8"), true);
        compare(BigInteger.create("10").multiply(BigInteger.fraction(0.8) ), "8");
    }

    function test_divide() {
        compare(BigInteger.create("1231233141592653589793238462643383279").divide("9128391823091").equals("134879523738031326302006"), true);
    }

    function test_isNegative() {
        compare(BigInteger.create("1231233141592653589793238462643383279").isNegative(), false);
        compare(BigInteger.create("-1231233141592653589793238462643383279").isNegative(), true);
    }

    function test_compare() {
        compare(BigInteger.create("81723121231231233").compare("123123123"), 1);
        compare(BigInteger.create("81723121231231233").greater("123123123"), true);
        compare(BigInteger.create("81723121231231233").greaterOrEquals("123123123"), true);
        compare(BigInteger.create("81723121231231233").greaterOrEquals("81723121231231233"), true);
        compare(BigInteger.create("81723121231231233").greaterOrEquals("81723121231231234"), false);

        compare(BigInteger.compare("123123123","81723121231231233") , -1);
        compare(BigInteger.create("123123123").lesser("81723121231231233"), true);
        compare(BigInteger.create("123123123").lesserOrEquals("81723121231231233"), true);

        compare(BigInteger.compare("81723121231231233","81723121231231233") , 0);
        compare(BigInteger.equals("81723121231231233","81723121231231233") , true);
        compare(BigInteger.create("81723121231231233").lesserOrEquals("81723121231231233") , true);
        compare(BigInteger.create("81723121231231233").greaterOrEquals("81723121231231233"), true);
    }

    function test_abs() {
        compare(BigInteger.abs("-12312312381723121231231233").equals("12312312381723121231231233"), true);
        compare(BigInteger.abs("12312312381723121231231233").equals("12312312381723121231231233"), true);
    }

    function test_gcd() {
        compare(BigInteger.gcd("-7","3").equals("1"), true);
        compare(BigInteger.gcd("9","3").equals("3"), true);
    }

    function test_pow() {
        compare(BigInteger.pow("23","99").equals("647225717667958234512676373328684966608135637121798638546825574314018838197362232702277832316406382792759851833889013515631314023038087"), true);
    }

    function test_fraction() {
        var f = BigInteger.fraction("0.125");

        compare(f[0].equals("1"), true);
        compare(f[1].equals("8"), true);

        f = BigInteger.fraction("0.8");
        compare(f[0].equals("4"), true);
        compare(f[1].equals("5"), true);

        f = BigInteger.fraction("0.8", false);
        compare(f[0].equals("8"), true);
        compare(f[1].equals("10"), true);
    }

}
