#include <QQmlEngine>
#include <QtQml>
#include "BigInteger.hh"
#include "bigintegerwrapper.h"
#include "BigIntegerUtils.hh"

BigIntegerWrapper::BigIntegerWrapper(QObject *parent) : QObject(parent)
{

}

QString BigIntegerWrapper::multiply(const QString &a, const QString &b)
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
