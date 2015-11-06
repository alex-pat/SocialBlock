#include "manager.h"
#include <QFile>
#include <QMessageBox>

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
    QFile hosts ("/etc/hosts");
    if ( hosts.open( QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append) == false ) {
        cerr << "Cannot open /etc/hosts" << endl;
        return;
    }
    QByteArray rawHosts = hosts.readAll();
    QString strHosts = rawHosts.toStdString();
    QStringList listHosts = strHosts.split('\n');


    QStringListIterator iter = listHosts.begin(),
                        end = listHosts.end();



    while ( iter.hasNext() ) {

    }
}
