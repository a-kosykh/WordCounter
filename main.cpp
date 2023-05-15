#include "controller.h"

#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Controller c1;
    QQmlApplicationEngine engine;
    const QUrl url("qrc:/main.qml");
    engine.rootContext()->setContextProperty("histogram_ctrl", QVariant::fromValue(&c1));
    engine.load(url);


    return app.exec();
}
