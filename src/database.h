#ifndef DATABASE_H
#define DATABASE_H

#include <QVector>
#include "src/profile.h"

class Database
{
    friend class Manager;

    QVector < Profile* > profiles;
    int currentProfile;
public:
    Database();
    ~Database();

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

#include "src/manager.h"

#endif // DATABASE_H
