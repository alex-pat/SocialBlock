#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include "connector.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qmlRegisterType<Connector>("SocialBlock.connector",1,0,"SBConnector");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/mainwindow.qml")));



    QApplication::setQuitOnLastWindowClosed(false);

    return a.exec();
}
