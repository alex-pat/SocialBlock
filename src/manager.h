#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>
#include <src/profile.h>

class Manager
{
    QVector < Profile* > profiles;
    int currentProfile;
public:
    Manager();
    void addProfile ( Profile* newProfile );
    void deleteProfile ( int index );
    void setCurrentProfile (int index );
    void startBlock ();
    void stopBlock();
};

#endif // MANAGER_H
