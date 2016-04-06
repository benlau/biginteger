#ifndef BIGINTEGERWRAPPER_H
#define BIGINTEGERWRAPPER_H

#include <QObject>

class BigIntegerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit BigIntegerWrapper(QObject *parent = 0);

signals:

public slots:

    QString multiply(const QString& a, const QString& b) const;

    QString add(const QString& a, const QString& b) const;

    QString minus(const QString& a, const QString& b) const;

    QString divide(const QString& a, const QString& b) const;

    QString abs(const QString &value) const;

    bool isNegative(const QString &value) const;

    int compare(const QString& a, const QString& b) const;

    bool greater(const QString& a, const QString& b) const;

    bool greaterOrEquals(const QString& a, const QString& b) const;

    bool lesser(const QString& a, const QString& b) const;

    bool lesserOrEquals(const QString& a, const QString& b) const;

    bool equals(const QString& a, const QString& b) const;

};

#endif // BIGINTEGERWRAPPER_H
