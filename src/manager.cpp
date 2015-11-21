#include "manager.h"

Manager::Manager() :
    currentProfile (0)
{ }

void Manager::addProfile(Profile *newProfile) {
    profiles.push_back( newProfile );
}

void Manager::deleteProfile(int index) {
    delete profiles[ index ];
    QVectorIterator < Profile* > iter;
    iter = profiles.begin();
    for ( int i = 0; i < index; i++ )
        iter++;
    cards.erase( iter );
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
