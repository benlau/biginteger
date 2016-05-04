#include <QQmlComponent>
#include <QtCore>
#include "BigInteger.hh"
#include "bigintegerobject2.h"
#include "BigIntegerUtils.hh"
#include "bigintegermath.h"

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

BigIntegerObject2::BigIntegerObject2(QObject *parent) : QObject(parent)
{
    m_engine = 0;
}

QQmlEngine *BigIntegerObject2::engine() const
{
    return m_engine;
}

void BigIntegerObject2::setEngine(QQmlEngine *engine)
{
    m_engine = engine;

    if (!m_engine) {
        return;
    }

    creator = loadJavascript(m_engine, "qrc:///BigInteger/js/biginteger.js","create");
}

QJSValue BigIntegerObject2::create()
{
    QJSValueList args;

    return creator.call(args);
}

QJSValue BigIntegerObject2::_multiply(QJSValue a, QJSValue value) const
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

        // Don't waste time to do normalization
        BigIntegerMath::fraction(value.toNumber(),numerator, denominator, false);
        v1 = v1 * numerator / denominator;

    } else {
        v2 = toBigInteger(value);
        v1 *= v2;
    }

    return m_engine->toScriptValue<BigInteger>(v1);
}

QJSValue BigIntegerObject2::_divide(QJSValue a, QJSValue b) const
{
    BigInteger v1,v2;
    v1 = toBigInteger(a);
    v2 = toBigInteger(b);

    v1 /= v2;

    return m_engine->toScriptValue<BigInteger>(v1);
}

bool BigIntegerObject2::isNegative(QJSValue a) const
{
    BigInteger v1;
    v1 = toBigInteger(a);

    return (v1.getSign() == BigInteger::negative);
}

QJSValue BigIntegerObject2::create(QJSValue value)
{
    QJSValueList args;
    args << value;

    return creator.call(args);
}

QJSValue BigIntegerObject2::fraction(qreal input, bool normalize)
{
    BigInteger numerator,denominator;

    BigIntegerMath::fraction(input,numerator,denominator, normalize);

    QJSValue res = m_engine->newArray(2);

    res.setProperty("0", create(m_engine->toScriptValue<BigInteger>(numerator)));
    res.setProperty("1", create(m_engine->toScriptValue<BigInteger>(denominator)));

    return res;
}

QVariant BigIntegerObject2::_createValue(QJSValue value) const
{
    BigInteger integer;

    if (value.isString()) {
        integer = stringToBigInteger(value.toString().toStdString());
    } else if (value.isNumber()) {
        integer = BigInteger((int) value.toNumber());
    }

    return QVariant::fromValue<BigInteger>(integer);
}

bool BigIntegerObject2::_equals(QJSValue a, QJSValue b) const
{
    BigInteger v1,v2;
    v1 = toBigInteger(a);
    v2 = toBigInteger(b);

    return v1 == v2;
}

QString BigIntegerObject2::_toString(QJSValue a) const
{
    BigInteger v;
    v = toBigInteger(a);

    return QString::fromStdString(bigIntegerToString(v));
}


