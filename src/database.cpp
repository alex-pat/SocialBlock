#include "database.h"
#include <QFile>
#include <iostream>

Database::Database () :
    currentProfile (0)
{ }

Database::~Database () {
    foreach (Profile* element, profiles)
        delete element;
}

void Database::addProfile(Profile *newProfile) {
    profiles.push_back( newProfile );
}

void Database::deleteProfile(int index) {
    delete profiles[ index ];
    profiles.remove(index);
}

void Database::setCurrentProfile(int index) {
    currentProfile = index;
}

void Database::startBlock() {
    if ( profiles.isEmpty() )
        return;
    profiles[currentProfile]->removeFromHosts();
    profiles[currentProfile]->writeToHosts();

}
void Database::stopBlock() {
    if ( profiles.isEmpty() )
        return;
    profiles[currentProfile]->removeFromHosts( false );
}

void Database::saveData() {
    QFile settings (".sbsettings");
    if ( settings.open ( QIODevice::WriteOnly ) == false ) {
        std::cerr << "Cannot open settings" << std::endl;
        exit(1);
    }
    QDataStream stngs (&settings);
    int size = profiles.size();
    stngs << size
          << currentProfile;
    for (int i = 0; i < size; i++)
        stngs << *(profiles[i]);
}

void Database::loadData() {
    QFile settings (".sbsettings");
    if ( settings.exists() == false ) {
        writeDefaults();
        return;
    }
    if ( settings.open ( QIODevice::ReadOnly ) == false ) {
        std::cerr << "Cannot open settings" << std::endl;
        exit(1);
    }
    QDataStream stngs (&settings);
    int size;
    stngs >> size
          >> currentProfile;
    for (int i = 0; i < size; i++) {
        Profile* prof = new Profile;
        stngs >> prof;
        profiles.push_back( prof );
    }
}

int Database::getCurrentNumber() const {
    return currentProfile;
}

void Database::writeDefaults() {
    currentProfile = 0;
    Profile* hastilyWork = new Profile;
    hastilyWork->setName("Hastily work");
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(9,0);
        interv->setEndTime(12,0);
        hastilyWork->addInterval(i, interv);
    }
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(14,0);
        interv->setEndTime(17,0);
        hastilyWork->addInterval(i, interv);
    }
    profiles.push_back(hastilyWork);

    Profile* passableWork = new Profile;
    passableWork->setName("Passable work");
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(8,0);
        interv->setEndTime(12,0);
        passableWork->addInterval(i, interv);
    }
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(13,0);
        interv->setEndTime(17,0);
        passableWork->addInterval(i, interv);
    }
    profiles.push_back(passableWork);

    Profile* stakhanov = new Profile;
    stakhanov->setName("Stakhanov");
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(7,0);
        interv->setEndTime(19,0);
        stakhanov->addInterval(i, interv);
    }
    profiles.push_back(stakhanov);

    Profile* sociopath = new Profile;
    sociopath->setName("sociopath");
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime(0,0);
        interv->setEndTime(23,59);
        sociopath->addInterval(i, interv);
    }
    profiles.push_back(sociopath);

    saveData();
}

bool Database::isBlockedNow() const {
    return profiles[currentProfile]->isBlockedNow();
}
