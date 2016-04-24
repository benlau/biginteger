#include <QString>
#include <QtTest>
#include "BigInteger.hh"
#include "BigIntegerUtils.hh"
#include "bigintegerunittests.h"
#include "bigintegermath.h"

BigIntegerUnitTests::BigIntegerUnitTests()
{
}

void BigIntegerUnitTests::initTestCase()
{
}

void BigIntegerUnitTests::cleanupTestCase()
{
}

void BigIntegerUnitTests::basicCalculation()
{
    BigInteger a,b,c;
    a = stringToBigInteger(std::string("3141592653589793238462643383279"));
    b = a;

    c = a * b;
    QString result;
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "9869604401089358618834490999872991420714831834895839696791841");

    a = stringToBigInteger(std::string("1231233141592653589793238462643383279"));
    b = stringToBigInteger(std::string("9128391823091"));

    c = a / b;

    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "134879523738031326302006");

    a = stringToBigInteger(std::string("1231233141592653589793238462643383279"));
    b = stringToBigInteger(std::string("1231233141592653589793238462643383280"));

    c = a - b;

    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "-1");
}

void BigIntegerUnitTests::pow()
{
    BigInteger a,c;
    int b;
    a = 13;
    b = 9;

    c = BigIntegerMath::pow(a, b);

    QString result;
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "10604499373");

    b = 10;

    c = BigIntegerMath::pow(a, b);
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "137858491849");

    a = 23;
    b = 20;

    c = BigIntegerMath::pow(a, b);
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "1716155831334586342923895201");

    a = 23;
    b = 99;

    c = BigIntegerMath::pow(a, b);
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "647225717667958234512676373328684966608135637121798638546825574314018838197362232702277832316406382792759851833889013515631314023038087");

}
