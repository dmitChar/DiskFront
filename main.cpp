#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QApplication>

#include "Controllers/AppController.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    AppController controller;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);
    engine.rootContext()->setContextProperty("AuthController", controller.getAuth());
    engine.rootContext()->setContextProperty("FileController", controller.getFileCtrl());
    engine.rootContext()->setContextProperty("User", controller.getUserModel());
    engine.rootContext()->setContextProperty("FilesModel", controller.getFilesModel());
    engine.rootContext()->setContextProperty("TransferModel", controller.getTransferModel());
    engine.rootContext()->setContextProperty("ProxyModel", controller.getProxyModel());

    qmlRegisterSingletonType(QUrl("qrc:/qml/AppTheme.qml"), "AppTheme", 1, 0, "AppTheme");
    qmlRegisterType<FileModel>("fileModel", 1, 0, "FileModel");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
