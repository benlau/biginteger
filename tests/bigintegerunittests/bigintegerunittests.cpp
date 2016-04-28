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

    a = 10;
    b = 1;
    c = BigIntegerMath::pow(a, b);
    result = QString::fromStdString(bigIntegerToString(c));
    QVERIFY(result == "10");

}

void BigIntegerUnitTests::gcd()
{
    QFETCH(QString, a);
    QFETCH(QString, b);
    QFETCH(QString, c);

    BigInteger bigA, bigB, bigC;

    bigA = stringToBigInteger(a.toStdString());
    bigB = stringToBigInteger(b.toStdString());

    bigC = BigIntegerMath::gcd(bigA, bigB);
    QString result = QString::fromStdString(bigIntegerToString(bigC));

    if (c != result) {
        qDebug() << "Expected: " << c;
        qDebug() << "Actual: " << result;
    }
    QVERIFY(c == result);
}

void BigIntegerUnitTests::gcd_data()
{
    QTest::addColumn<QString>("a");
    QTest::addColumn<QString>("b");
    QTest::addColumn<QString>("c");

    QTest::newRow("") << "7" << "3" << "1";
    QTest::newRow("") << "-7" << "3" << "1";

    QTest::newRow("") << "99999" << "100002" << "3";
    QTest::newRow("") << "100002" << "99999" << "3";

}

void BigIntegerUnitTests::fraction()
{
    QFETCH(qreal, decimal);
    QFETCH(QString, numerator);
    QFETCH(QString, denominator);

    BigInteger bigN(1), bigD(1);

    BigIntegerMath::fraction(decimal, bigN, bigD);

    QString rN, rD;

    rN = QString::fromStdString(bigIntegerToString(bigN));
    rD = QString::fromStdString(bigIntegerToString(bigD));

    if (rN != numerator) {
        qDebug() << "Different:" << rN << numerator;
    }
    QVERIFY(rN == numerator);
    QVERIFY(rD == denominator);
}

void BigIntegerUnitTests::fraction_data()
{
    QTest::addColumn<qreal>("decimal");
    QTest::addColumn<QString>("numerator");
    QTest::addColumn<QString>("denominator");

    QTest::newRow("") << 125.0 << "125" << "1";
    QTest::newRow("") << 0.125 << "1" << "8";
    QTest::newRow("") << 1.07 << "107" << "100";
    QTest::newRow("") << -1.07 << "-107" << "100";
    QTest::newRow("") << 0.0 << "0" << "1";
    QTest::newRow("") << 0.8 << "4" << "5";

}
