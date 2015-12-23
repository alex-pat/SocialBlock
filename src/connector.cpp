#include "connector.h"
#include "src/profile.h"

Connector::Connector( QObject *parent ) : QObject(parent) {
    manag = new Manager;

    manag->loadData();

    QAction* actionShow = new QAction ("SocialBlock", this);
    connect(actionShow, SIGNAL(triggered(bool)),
            this,       SIGNAL(openApplication()));

    QAction* actionExit = new QAction ("Exit", this);
    connect(actionExit, SIGNAL(triggered(bool)),
            this,       SLOT(exitTriggered()));

    iconMenu = new QMenu;
    iconMenu->addAction(actionShow);
    iconMenu->addAction(actionExit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(iconMenu);
    trayIcon->setToolTip("SocialBlock");
    trayIcon->setIcon(QPixmap("://logo.svg"));
    trayIcon->show();

    isblocked = false;
    istracked = false;

    timer = new QTimer (this);
    connect (timer, SIGNAL(timeout()),
             this,  SLOT (timeout()));
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
    if ( isblocked == false )
        emit exitApplication();
}

void Connector::deleteInterval(int profile, int day, int interv) {
    manag->profiles[profile]->deleteInterval(day, interv);
}

void Connector::save() {
    manag->saveData();
}

void Connector::timeout() {
    if ( manag->isBlockedNow() == isblocked )
        return;
    isblocked = !isblocked;
    if ( isblocked == true )
        manag->startBlock();
    else
        manag->stopBlock();
    emit blockedChanged();
}

void Connector::trackTriggered() {
    if (istracked == true )
        timer->stop();
    else {
        timer->start( 60*1000 );
        timeout();
    }
    istracked = !istracked;
    emit istrackedChanged();
}

bool Connector::isBlocked() const {
    return isblocked;
}

bool Connector::isTracked() const {
    return istracked;
}

void Connector::setCurrentProfileNumber( int profile ) {
    manag->setCurrentProfile( profile );
}

void Connector::addProfile(QString name, QString sites) {
    Profile* newProfile = new Profile;
    newProfile->setName(name);
    QStringList urlList = sites.split('\n');
    newProfile->fillStandartList( urlList );
    manag->addProfile(newProfile);
}

void Connector::deleteProfile(int index) {
    manag->deleteProfile( index );
    emit exitApplication();
}

void Connector::addInterval(int profile,    int day,
                            int hoursBegin, int minuteBegin,
                            int hoursEnd,   int minuteEnd,
                            QString sites) {
    BlockInterval* newInterv = new BlockInterval;
    newInterv->setBeginTime(hoursBegin, minuteBegin);
    newInterv->setEndTime  (hoursEnd,   minuteEnd);
    QStringList urlList = sites.split('\n');
    newInterv->setNewAdresses( urlList );
    manag->profiles[profile]->addInterval(day, newInterv);
}
