#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include "manager.h"

class Connector : public QObject
{
    Q_OBJECT

    Manager* manag;
    bool blocked;
    QSystemTrayIcon* trayIcon;
    QMenu* iconMenu;

    Q_PROPERTY(bool blocked READ isBlocked NOTIFY blockedChanged)
public:
    Connector( QObject *parent = 0);
    ~Connector();

    Q_INVOKABLE QStringList getProfileNames();
    Q_INVOKABLE int getProfilesCount();
    Q_INVOKABLE int getCurrentProfileNumber();
    Q_INVOKABLE QStringList getTimesList( int profile, int day);
    Q_INVOKABLE QStringList getSitesList (int profile, int day);
    Q_INVOKABLE void deleteInterval( int profile, int day, int interv);
    Q_INVOKABLE void save();

    bool isBlocked();

signals:
    //void settingsLoaded();
    void exitApplication();
    void openApplication();
    void blockedChanged();

public slots:
    void exitTriggered();
};

#endif // CONNECTOR_H
