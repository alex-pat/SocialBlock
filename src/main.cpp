#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include "manager.h"

extern bool isRestart;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qmlRegisterType<Manager>("SocialBlock.manager",1,0,"SBManager");

    QApplication::setQuitOnLastWindowClosed(false);

    QQmlApplicationEngine engine;
    while ( isRestart == true ) {
    engine.load(QUrl(QStringLiteral("qrc:/mainwindow.qml")));


        a.exec();
}
    return 0;
}
