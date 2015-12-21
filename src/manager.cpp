#include "manager.h"
#include <QFile>
#include <iostream>

Manager::Manager( /*QObject* parent*/ ) :
    //QObject (parent),
    currentProfile (0)
{ }

//void Manager::setProfileNames() {
//    QStringList resultList;
//    int size = profiles.size();
//    for (int i = 0; i < size; i++)
//        resultList << profiles[i]->getName();
//    profileNames.setStringList(resultList);
//}

//QStringListModel Manager::getDays() {
//    QStringList days;
//    days << "Monday"
//         << "Tuesday"
//         << "Wednesday"
//         << "Thursday"
//         << "Friday"
//         << "Saturday"
//         << "Sunday";
//}

void Manager::addProfile(Profile *newProfile) {
    profiles.push_back( newProfile );
}

void Manager::deleteProfile(int index) {
    delete profiles[ index ];
    profiles.remove(index);
}

void Manager::setCurrentProfile(int index) {
    currentProfile = index;
}

void Manager::startBlock() {
    if ( profiles.isEmpty() )
        return;
    profiles[currentProfile]->removeFromHosts();
    profiles[currentProfile]->writeToHosts();

}

void Manager::stopBlock() {
    if ( profiles.isEmpty() )
        return;
    profiles[currentProfile]->removeFromHosts();
}

void Manager::saveData() {
    QFile settings ("sbsettings");
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

void Manager::loadData() {
    QFile settings ("sbsettings");
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

int Manager::getCurrentNumber() const {
    return currentProfile;
}
