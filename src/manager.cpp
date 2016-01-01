#include "manager.h"
#include "src/profile.h"

bool isRestart = true;

Manager::Manager( QObject *parent ) : QObject(parent) {
    base = new Database;

    base->loadData();

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
    isRestart = false;
}

Manager::~Manager() {
    base->saveData();
    delete base;
    delete iconMenu;
    trayIcon->hide();
    delete trayIcon;
    delete timer;
}

QStringList Manager::getProfileNames() {
    QStringList resultNames;
    int count = getProfilesCount();
    for (int i = 0; i < count; i++)
        resultNames << base->profiles[i]->getName();
    return resultNames;
}

int Manager::getProfilesCount() {
    return base->profiles.size();
}

int Manager::getCurrentProfileNumber() {
    return base->getCurrentNumber();
}

QStringList Manager::getTimesList(int profile, int day) {
    return base->profiles[profile]->getIntervals(day);
}

QStringList Manager::getSitesList(int profile, int day) {
    return base->profiles[profile]->getSites(day);
}

void Manager::exitTriggered() {
    if ( isblocked == false )
        emit exitApplication();
}

void Manager::deleteInterval(int profile, int day, int interv) {
    base->profiles[profile]->deleteInterval(day, interv);
}

void Manager::save() {
    base->saveData();
}

void Manager::timeout() {
    if ( base->isBlockedNow() == isblocked )
        return;
    isblocked = !isblocked;
    if ( isblocked == true )
        base->startBlock();
    else
        base->stopBlock();
    emit blockedChanged();
}

void Manager::trackTriggered() {
    if (istracked == true )
        timer->stop();
    else {
        timer->start( 60*1000 );
        timeout();
    }
    iconMenu->actions()[1]->setEnabled(istracked);
    istracked = !istracked;
    emit istrackedChanged();
}

bool Manager::isBlocked() const {
    return isblocked;
}

bool Manager::isTracked() const {
    return istracked;
}

void Manager::setCurrentProfileNumber( int profile ) {
    base->setCurrentProfile( profile );
}

void Manager::addProfile(QString name, QString sites) {
    Profile* newProfile = new Profile;
    newProfile->setName(name);
    QStringList urlList = sites.split('\n');
    newProfile->fillStandartList( urlList );
    base->addProfile(newProfile);
    base->saveData();
    isRestart = true;
    emit exitApplication();
}

void Manager::deleteProfile(int index) {
    base->deleteProfile( index );
    base->saveData();
    isRestart = true;
    emit exitApplication();
}

void Manager::addInterval(int profile,    int day,
                            int hoursBegin, int minuteBegin,
                            int hoursEnd,   int minuteEnd,
                            QString sites) {
    BlockInterval* newInterv = new BlockInterval;
    newInterv->setBeginTime(hoursBegin, minuteBegin);
    newInterv->setEndTime  (hoursEnd,   minuteEnd);
    QStringList urlList = sites.split('\n');
    newInterv->setNewAdresses( urlList );
    base->profiles[profile]->addInterval(day, newInterv);
}

void Manager::close() {
    trayIcon->hide();
}
