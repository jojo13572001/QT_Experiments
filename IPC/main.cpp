#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <cserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    CServer test;
    if(!test.mpTryToConnectToServer()){
        test.RunServer();
    }
    return app.exec();
}
