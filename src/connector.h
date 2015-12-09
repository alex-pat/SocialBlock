#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include "manager.h"

class Connector : public QObject
{
    Q_OBJECT

    Manager* manag;
public:
    explicit Connector( QObject *parent = 0);
    ~Connector();

    Q_INVOKABLE QStringList getProfileNames();
    Q_INVOKABLE int getProfilesCount();

signals:

public slots:
};

#endif // CONNECTOR_H
