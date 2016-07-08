#include <QQmlEngine>
#include <QtQml>
#include "bigintegerobject.h"
#include "bigintegerobject2.h"

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    BigIntegerObject* object = new BigIntegerObject();
    return object;
}


static QObject *provider2(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    BigIntegerObject2* object = new BigIntegerObject2();
    object->setEngine(engine);
    return object;
}

static void registerQmlTypes() {
    qmlRegisterSingletonType<BigIntegerObject>("BigInteger", 1, 0, "BigInteger", provider);
    qmlRegisterSingletonType<BigIntegerObject2>("BigInteger", 2, 0, "BigInteger", provider2);
}

Q_COREAPP_STARTUP_FUNCTION(registerQmlTypes)
