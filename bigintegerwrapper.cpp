#include <QQmlEngine>
#include <QtQml>
#include "BigInteger.hh"
#include "bigintegerwrapper.h"
#include "BigIntegerUtils.hh"

BigIntegerWrapper::BigIntegerWrapper(QObject *parent) : QObject(parent)
{

}

QString BigIntegerWrapper::add(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 + v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerWrapper::minus(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 - v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerWrapper::divide(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 / v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerWrapper::abs(const QString &value) const
{
    if (isNegative(value)) {
        BigInteger v = stringToBigInteger(value.toStdString());
        v.flipSign();
        return QString::fromStdString(bigIntegerToString(v));
    } else {
        return  value;
    }
}

bool BigIntegerWrapper::isNegative(const QString &value) const
{
    return value.size() > 0 && value[0] == QChar('-');
}

int BigIntegerWrapper::compare(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1.compareTo(v2);
}

bool BigIntegerWrapper::greater(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 > v2;
}

bool BigIntegerWrapper::greaterOrEquals(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 >= v2;
}

bool BigIntegerWrapper::lesser(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 < v2;
}

bool BigIntegerWrapper::lesserOrEquals(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 <= v2;
}

bool BigIntegerWrapper::equals(const QString &a, const QString &b) const
{
    return a == b;
}

QString BigIntegerWrapper::multiply(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 * v2;

    return QString::fromStdString(bigIntegerToString(r));
}


static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    BigIntegerWrapper* object = new BigIntegerWrapper();
    return object;
}

class BigIntegerWrapperRegistrationHelper {
public:
    BigIntegerWrapperRegistrationHelper() {
        qmlRegisterSingletonType<BigIntegerWrapper>("BigInteger", 1, 0, "BigInteger", provider);
    }
};

static BigIntegerWrapperRegistrationHelper registerHelper;
