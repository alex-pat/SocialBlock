#ifndef PROFILE_H
#define PROFILE_H

#include <QList>
#include <src/blockinterval.h>

class Profile
{
    QList < BlockInterval* > week[7];
public:
    Profile();
    void addInterval (int day, BlockInterval* newInterv);
    // void deleteInterval (int day, int index);
    // дописать
};

#endif // PROFILE_H
