#ifndef BIGINTEGEROBJECT2_H
#define BIGINTEGEROBJECT2_H

#include <QObject>
#include <QJSValue>
#include <QVariant>
#include <QQmlEngine>

/// BigInteger 2.0

class BigIntegerObject2 : public QObject
{
    Q_OBJECT
public:
    explicit BigIntegerObject2(QObject *parent = 0);

    QQmlEngine *engine() const;

    void setEngine(QQmlEngine *engine);

signals:

public slots:
    QJSValue create();

    QJSValue create(QJSValue value);

    QJSValue fraction(qreal input, bool normalize = true);

    QVariant _createValue(QJSValue value) const;

    bool equals(QJSValue a, QJSValue b) const;

    QString _toString(QJSValue a) const;

    QJSValue _add(QJSValue a, QJSValue value) const;

    QJSValue _multiply(QJSValue a, QJSValue value) const;

    QJSValue _divide(QJSValue a, QJSValue b) const;

    bool isNegative(QJSValue a) const;

    int compare(QJSValue a, QJSValue b) const;

    QJSValue abs(QJSValue a);

    QJSValue gcd(QJSValue a, QJSValue b);

    QJSValue pow(QJSValue base, int exponent);

private:
    QQmlEngine* m_engine;
    QJSValue creator;
};

#endif // BIGINTEGEROBJECT2_H
