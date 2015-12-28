#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include "database.h"

class Manager : public QObject
{
    Q_OBJECT

    Database* base;
    bool isblocked;
    bool istracked;
    QSystemTrayIcon* trayIcon;
    QMenu* iconMenu;
    QTimer* timer;

    Q_PROPERTY(bool isblocked READ isBlocked NOTIFY blockedChanged)
    Q_PROPERTY(bool istracked READ isTracked NOTIFY istrackedChanged)

public:
    Manager( QObject *parent = 0);
    ~Manager();

    Q_INVOKABLE QStringList getProfileNames();
    Q_INVOKABLE int getProfilesCount();
    Q_INVOKABLE int getCurrentProfileNumber();
    Q_INVOKABLE QStringList getTimesList( int profile, int day);
    Q_INVOKABLE QStringList getSitesList (int profile, int day);
    Q_INVOKABLE void addProfile ( QString name, QString sites);
    Q_INVOKABLE void deleteProfile (int profile);
    Q_INVOKABLE void addInterval (int profile,    int day,
                                  int hoursBegin, int minuteBegin,
                                  int hoursEnd,   int minuteEnd,
                                  QString sites);
    Q_INVOKABLE void setCurrentProfileNumber( int profile );
    Q_INVOKABLE void deleteInterval( int profile, int day, int interv);
    Q_INVOKABLE void save ();
    Q_INVOKABLE void close ();

    bool isBlocked() const;
    bool isTracked() const;
signals:
    void exitApplication();
    void openApplication();
    void blockedChanged();
    void istrackedChanged();

public slots:
    void exitTriggered ();
    void timeout ();
    void trackTriggered();
};

#endif // MANAGER_H
