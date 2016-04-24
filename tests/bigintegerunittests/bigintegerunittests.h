#ifndef BIGINTEGERUNITTESTS_H
#define BIGINTEGERUNITTESTS_H

#include <QObject>

class BigIntegerUnitTests : public QObject
{
    Q_OBJECT

public:
    BigIntegerUnitTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void basicCalculation();

    void pow();

    void gcd();
    void gcd_data();

    void fraction();
    void fraction_data();

};

#endif // BIGINTEGERUNITTESTS_H
