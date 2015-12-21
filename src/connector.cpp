#include "connector.h"

#include "src/profile.h"

Connector::Connector( QObject *parent ) : QObject(parent) {
    manag = new Manager;
//    Profile *newprof ;
//    for (int i = 0; i < 3; i++) {
//        newprof = new Profile;
//        newprof->setName(QString::number(i));
//        manag->addProfile(newprof);
//    }

    manag->loadData();
    //manag->saveData();
    //emit settingsLoaded();

    QAction* actionShow = new QAction ("SocialBlock", this);
    connect(actionShow, SIGNAL(triggered(bool)),
            this,       SLOT(openTriggered()));

    QAction* actionExit = new QAction ("Exit", this);
    connect(actionExit, SIGNAL(triggered(bool)),
            this,       SIGNAL(openApplication()));

    iconMenu = new QMenu;
    iconMenu->addAction(actionShow);
    iconMenu->addAction(actionExit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(iconMenu);
    trayIcon->setToolTip("SocialBlock");
    trayIcon->setIcon(QPixmap("://logo.svg"));
    trayIcon->show();


}

Connector::~Connector() {
    manag->saveData();
    delete manag;
}

QStringList Connector::getProfileNames() {
    QStringList resultNames;
    int count = getProfilesCount();
    for (int i = 0; i < count; i++)
        resultNames << manag->profiles[i]->getName();
    return resultNames;
}

int Connector::getProfilesCount() {
    return manag->profiles.size();
}

int Connector::getCurrentProfileNumber() {
    return manag->getCurrentNumber();
}

QStringList Connector::getTimesList(int profile, int day) {
    return manag->profiles[profile]->getIntervals(day);
}

QStringList Connector::getSitesList(int profile, int day) {
    return manag->profiles[profile]->getSites(day);
}

void Connector::exitTriggered() {
    if ( blocked == false )
        emit exitApplication();
}

bool Connector::isBlocked() {
    return blocked;
}

void Connector::deleteInterval(int profile, int day, int interv) {
    manag->profiles[profile]->deleteInterval(day, interv);
}

void Connector::save() {
    manag->saveData();
}
