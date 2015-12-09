#include "connector.h"

#include "src/profile.h"

Connector::Connector( QObject *parent ) : QObject(parent) {
    manag = new Manager;
    Profile *newprof ;
    for (int i = 0; i < 3; i++) {
        newprof = new Profile;
        newprof->setName(QString::number(i));
        manag->addProfile(newprof);
    }
}

Connector::~Connector() {
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
