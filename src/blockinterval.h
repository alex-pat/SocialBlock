#ifndef BLOCKINTERVAL_H
#define BLOCKINTERVAL_H

#include <QTime>

class BlockInterval
{
    QTime timeBegin;
    QTime timeEnd;
    QStringList adresses;
public:
    BlockInterval();
    void setBeginTime (int hh, int mm);
    void setEndTime (int hh, int mm);
    void setNewAdresses (QStringList &newAddr);
    bool isIncludeTime ( QTime& time );
};

#endif // BLOCKINTERVAL_H
