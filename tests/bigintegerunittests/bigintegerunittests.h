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
    void function();
    void qml();
};

#endif // BIGINTEGERUNITTESTS_H
