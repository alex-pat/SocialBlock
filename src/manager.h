#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>
#include "src/profile.h"

class Manager
{
    friend class Connector;

    QVector < Profile* > profiles;
    int currentProfile;
public:
    Manager();

    void addProfile ( Profile* newProfile );
    void deleteProfile ( int index );
    void setCurrentProfile (int index );
    void startBlock ();
    void stopBlock();
    void saveData();
    void loadData();
    int getCurrentNumber () const;
    void writeDefaults ();
    bool isBlockedNow () const;

};

#include "src/connector.h"

#endif // MANAGER_H
