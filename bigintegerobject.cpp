#include <QQmlEngine>
#include <QtQml>
#include "BigInteger.hh"
#include "bigintegerobject.h"
#include "BigIntegerUtils.hh"

BigIntegerObject::BigIntegerObject(QObject *parent) : QObject(parent)
{

}

QString BigIntegerObject::add(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 + v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerObject::minus(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 - v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerObject::divide(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 / v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerObject::abs(const QString &value) const
{
    if (isNegative(value)) {
        BigInteger v = stringToBigInteger(value.toStdString());
        v.flipSign();
        return QString::fromStdString(bigIntegerToString(v));
    } else {
        return  value;
    }
}

bool BigIntegerObject::isNegative(const QString &value) const
{
    return value.size() > 0 && value[0] == QChar('-');
}

int BigIntegerObject::compare(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1.compareTo(v2);
}

bool BigIntegerObject::greater(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 > v2;
}

bool BigIntegerObject::greaterOrEquals(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 >= v2;
}

bool BigIntegerObject::lesser(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 < v2;
}

bool BigIntegerObject::lesserOrEquals(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    return v1 <= v2;
}

bool BigIntegerObject::equals(const QString &a, const QString &b) const
{
    return a == b;
}

QString BigIntegerObject::multiply(const QString &a, const QString &b) const
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

    BigIntegerObject* object = new BigIntegerObject();
    return object;
}

class BigIntegerWrapperRegistrationHelper {
public:
    BigIntegerWrapperRegistrationHelper() {
        qmlRegisterSingletonType<BigIntegerObject>("BigInteger", 1, 0, "BigInteger", provider);
    }
};

static BigIntegerWrapperRegistrationHelper registerHelper;
