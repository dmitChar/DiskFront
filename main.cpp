#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "Controllers/AppController.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    AppController controller;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);
    engine.rootContext()->setContextProperty("AuthController", controller.getAuth());
    //engine.rootContext()->setContextProperty("FileController", controller.ge);

    qmlRegisterSingletonType(QUrl("qrc:/qml/AppTheme.qml"), "AppTheme", 1, 0, "AppTheme");
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
