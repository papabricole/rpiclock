#include <QCursor>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "DHT11.h"

int
main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOverrideCursor(QCursor(Qt::BlankCursor));

    qmlRegisterType<DHT11>("com.dht11", 1, 0, "DHT11");

    QQmlApplicationEngine engine;
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

    // RpiClock clock(engine.rootObjects().first());

    return app.exec();
}
