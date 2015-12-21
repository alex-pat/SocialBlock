#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>
//#include <QObject>
#include "src/profile.h"

class Manager //: public QObject
{
    //Q_OBJECT

    friend class Connector;

//    QStringListModel profileNames;

    QVector < Profile* > profiles;
    int currentProfile;
public:
    Manager( /*QObject *parent = 0*/ );

//    Q_INVOKABLE QStringListModel getDays();

    void addProfile ( Profile* newProfile );
    void deleteProfile ( int index );
    void setCurrentProfile (int index );
    void startBlock ();
    void stopBlock();
    void saveData();
    void loadData();
    int getCurrentNumber() const;

//signals:
//    void profileNamesChanged(QStringList);
};

#include "src/connector.h"

#endif // MANAGER_H
