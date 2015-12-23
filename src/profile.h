#ifndef PROFILE_H
#define PROFILE_H

#include <QList>
#include <src/blockinterval.h>

class Profile
{
    QList < BlockInterval* > week[7];
    QString name;
public:
    Profile();
    void addInterval (int day, BlockInterval* newInterv);
    void deleteInterval (int day, int index);
    void changeInterval (int day, int index, BlockInterval& newInterv );
    void writeToHosts ( );
    void removeFromHosts ( bool isNow = true ); //isNow says that we data during blocking
    void setName ( QString newName );
    QString getName() const;
    QStringList getIntervals(int day) const;
    QStringList getSites (int day) const;
    bool isBlockedNow() const;
    void fillStandartList ( QStringList& urlList);

    friend QDataStream& operator<< (QDataStream& stream, Profile& prof);
    friend QDataStream& operator>> (QDataStream& stream, Profile* prof);
};

#endif // PROFILE_H
