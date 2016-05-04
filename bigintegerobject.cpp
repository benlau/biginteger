#include <QQmlEngine>
#include <QtQml>
#include "BigInteger.hh"
#include "bigintegerobject.h"
#include "BigIntegerUtils.hh"
#include "bigintegermath.h"

// Copy from QuickCross project
static QJSValue loadJavascript(QQmlEngine *engine, const QString &url, const QString &member)
{

    QString pattern  = "import QtQuick 2.0\nimport \"%1\" as JSObject;QtObject { property var object : JSObject.%2}";

    QString qml = pattern.arg(url).arg(member);

    QObject* holder = 0;

    QQmlComponent comp (engine);
    comp.setData(qml.toUtf8(),QUrl());
    holder = comp.create();

    if (!holder) {
        qWarning() << QString("QuickCross: Failed to load Javscript: %1").arg(url);
        qWarning() << QString("Error: ") << comp.errorString();
        return 0;
    }

    QJSValue object = holder->property("object").value<QJSValue>();
    holder->deleteLater();

    if (object.isError()) {
        qWarning() << QString("QuickCross: Failed to load Javascript: %1").arg(url);
        qWarning() << object.toString();
    }

    return object;
}

static BigInteger toBigInteger(QJSValue value) {
    BigInteger a;

    if (value.isString()) {
        a = stringToBigInteger(value.toString().toStdString());
    } else if (value.isNumber()) {
        a = BigInteger((int) value.toNumber());
    } else if (value.hasOwnProperty("value")) {
        a = value.property("value").toVariant().value<BigInteger>();
    } else {
        a = value.toVariant().value<BigInteger>();
    }

    return a;
}

BigIntegerObject::BigIntegerObject(QObject *parent) : QObject(parent)
{
    m_engine = 0;
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

QStringList BigIntegerObject::fraction(qreal input) const
{
    BigInteger numerator, denominator;
    QStringList res;

    BigIntegerMath::fraction(input,numerator, denominator);

    res << QString::fromStdString(bigIntegerToString(numerator))
        << QString::fromStdString(bigIntegerToString(denominator));

    return res;
}

QString BigIntegerObject::gcd(const QString &a, const QString &b) const
{
    BigInteger v1,v2;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());

    BigInteger result = BigIntegerMath::gcd(v1,v2);

    return QString::fromStdString(bigIntegerToString(result));
}

QString BigIntegerObject::pow(const QString &a, int b) const
{
    BigInteger v1;

    v1 = stringToBigInteger(a.toStdString());

    BigInteger result = BigIntegerMath::pow(v1,b);

    return QString::fromStdString(bigIntegerToString(result));

}

QQmlEngine *BigIntegerObject::engine() const
{
    return m_engine;
}

void BigIntegerObject::setEngine(QQmlEngine *engine)
{
    m_engine = engine;

    if (!m_engine) {
        return;
    }

    creator = loadJavascript(m_engine, "qrc:///BigInteger/js/biginteger.js","create");
}

QJSValue BigIntegerObject::create()
{
    QJSValueList args;

    return creator.call(args);
}

QJSValue BigIntegerObject::_multiply(QJSValue a, QJSValue value) const
{
    BigInteger v1,v2;
    v1 = toBigInteger(a);

    if (value.isArray()) {
        BigInteger numerator,denominator;
        numerator = toBigInteger(value.property("0"));
        denominator = toBigInteger(value.property("1"));
        v1 = v1 * numerator / denominator;

    } else if (value.isNumber()) {

        BigInteger numerator,denominator;
        BigIntegerMath::fraction(value.toNumber(),numerator, denominator);
        v1 = v1 * numerator / denominator;

    } else {
        v2 = toBigInteger(value);
        v1 *= v2;
    }

    return m_engine->toScriptValue<BigInteger>(v1);
}

QJSValue BigIntegerObject::create(QJSValue value)
{
    QJSValueList args;
    args << value;

    return creator.call(args);
}

QString BigIntegerObject::multiply(const QString &a, const QString &b) const
{
    BigInteger v1,v2,r;

    v1 = stringToBigInteger(a.toStdString());
    v2 = stringToBigInteger(b.toStdString());
    r = v1 * v2;

    return QString::fromStdString(bigIntegerToString(r));
}

QString BigIntegerObject::multiply(const QString &a, const QStringList &fraction) const
{
    if (fraction.size() != 2) {
        qWarning() << "BigInteger::multiply(a, fraction) - Invalid fraction";
        return a;
    }

    BigInteger numerator,denominator,value;

    value = stringToBigInteger(a.toStdString());
    numerator = stringToBigInteger(fraction.at(0).toStdString());
    denominator = stringToBigInteger(fraction.at(1).toStdString());

    value *= numerator;
    value /= denominator;

    return QString::fromStdString(bigIntegerToString(value));
}

QString BigIntegerObject::multiply(const QString &a, qreal b) const
{
    BigInteger numerator, denominator,v;
    QString res;

    BigIntegerMath::fraction(b,numerator, denominator);

    v = stringToBigInteger(a.toStdString());

    v *= numerator;
    v /= denominator;

    res = QString::fromStdString(bigIntegerToString(v));

    return res;
}

QVariant BigIntegerObject::_createValue(QJSValue value) const
{
    BigInteger integer;

    if (value.isString()) {
        integer = stringToBigInteger(value.toString().toStdString());
    } else if (value.isNumber()) {
        integer = BigInteger((int) value.toNumber());
    }

    return QVariant::fromValue<BigInteger>(integer);
}

bool BigIntegerObject::_equals(QJSValue a, QJSValue b) const
{
    BigInteger v1,v2;
    v1 = toBigInteger(a);
    v2 = toBigInteger(b);

    return v1 == v2;
}

QString BigIntegerObject::_toString(QJSValue a) const
{
    BigInteger v;
    v = toBigInteger(a);

    return QString::fromStdString(bigIntegerToString(v));
}

QString BigIntegerObject::multiply(const QString &a, QJSValue value) const
{
    if (value.isString()) {
        return multiply(a, value.toString());
    } else if (value.isNumber()) {
        return multiply(a, value.toNumber());
    } else if (value.isArray() || value.isObject()) {
        QStringList fraction;
        fraction << value.property(0).toString()
                 << value.property(1).toString();
        return multiply(a, fraction);
    } else {
        qWarning() << "BigIntegerObject::multiply() - unsupported data type";
        return "NaN";
    }
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    BigIntegerObject* object = new BigIntegerObject();
    object->setEngine(engine);
    return object;
}

class BigIntegerWrapperRegistrationHelper {
public:
    BigIntegerWrapperRegistrationHelper() {
        qmlRegisterSingletonType<BigIntegerObject>("BigInteger", 1, 0, "BigInteger", provider);
    }
};

static BigIntegerWrapperRegistrationHelper registerHelper;
