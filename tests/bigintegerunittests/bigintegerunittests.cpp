#include <QString>
#include <QtTest>
#include "BigInteger.hh"
#include "BigIntegerUtils.hh"

class BigIntegerUnitTests : public QObject
{
    Q_OBJECT

public:
    BigIntegerUnitTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void function();
};

BigIntegerUnitTests::BigIntegerUnitTests()
{
}

void BigIntegerUnitTests::initTestCase()
{
}

void BigIntegerUnitTests::cleanupTestCase()
{
}

void BigIntegerUnitTests::function()
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

QTEST_APPLESS_MAIN(BigIntegerUnitTests)

#include "bigintegerunittests.moc"
