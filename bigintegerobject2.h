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

    QVariant _createValue(QJSValue value) const;

    bool _equals(QJSValue a, QJSValue b) const;

    QString _toString(QJSValue a) const;

    QJSValue _multiply(QJSValue a, QJSValue value) const;

private:
    QQmlEngine* m_engine;
    QJSValue creator;
};

#endif // BIGINTEGEROBJECT2_H
