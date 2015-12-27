#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include "manager.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qmlRegisterType<Manager>("SocialBlock.manager",1,0,"SBManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/mainwindow.qml")));
    QApplication::setQuitOnLastWindowClosed(false);

    return a.exec();
}
