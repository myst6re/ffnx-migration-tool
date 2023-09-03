#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "qmain.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    qmlRegisterType<QMain>("QMain", 1, 0, "QMain");
    
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ffnx-migration-tool", "Main");

    return app.exec();
}
