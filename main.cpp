#include "controller.h"

#include <QDebug>
#include <QString>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("TestOrg");
    app.setOrganizationDomain("TestDomain");
    QCoreApplication::setApplicationName("word-counter");
    QCoreApplication::setApplicationVersion("1.0");

    QString lineParam(argv[1]);
    int linesPerIteration = 0;
    if (!lineParam.isEmpty()) {
        linesPerIteration = lineParam.toInt();
    }

    Controller c1(linesPerIteration);
    QQmlApplicationEngine engine;
    const QUrl url("qrc:/main.qml");
    engine.rootContext()->setContextProperty("histogram_ctrl", QVariant::fromValue(&c1));
    engine.load(url);

    return app.exec();
}
