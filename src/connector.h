#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include "manager.h"

class Connector : public QObject
{
    Q_OBJECT

    Manager* manag;
    bool isblocked;
    bool istracked;
    QSystemTrayIcon* trayIcon;
    QMenu* iconMenu;
    QTimer* timer;

    Q_PROPERTY(bool isblocked READ isBlocked NOTIFY blockedChanged)
    Q_PROPERTY(bool istracked READ isTracked NOTIFY istrackedChanged)

public:
    Connector( QObject *parent = 0);
    ~Connector();

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
    void save ();
    void deleteInterval( int profile, int day, int interv);
    void trackTriggered();
    void setCurrentProfileNumber( int profile );
};

#endif // CONNECTOR_H
