import QtQuick 2.0
import QtTest 1.0
import BigInteger 1.0

TestCase {
    name: "CalculationTests"

    function test_multiply() {
        compare(BigInteger.multiply("3141592653589793238462643383279","3141592653589793238462643383279"), "9869604401089358618834490999872991420714831834895839696791841");
    }

}