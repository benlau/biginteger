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

    QString multiply(const QString& a, const QString& b);

};

#endif // BIGINTEGERWRAPPER_H
