#include <QCursor>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "qmlmqttclient.h"

int
main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    
    QQmlApplicationEngine engine;

    qmlRegisterType<QmlMqttClient>("MqttClient", 1, 0, "MqttClient");
    qmlRegisterUncreatableType<QmlMqttSubscription>("MqttClient", 1, 0, "MqttSubscription", QLatin1String("Subscriptions are read-only"));

    const QUrl url(QStringLiteral("qrc:/RpiClock.qml"));
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject* obj, const QUrl& objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
