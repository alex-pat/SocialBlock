#ifndef BLOCKINTERVAL_H
#define BLOCKINTERVAL_H

#include <QTime>

class BlockInterval
{
    QTime timeBegin;
    QTime timeEnd;
    QStringList addresses;
public:
    BlockInterval();
    void setBeginTime (int hh, int mm);
    void setEndTime (int hh, int mm);
    void setNewAdresses (QStringList &newAddr);
    bool isIncludeTime ( );
    QStringList& getAddresses();
    BlockInterval& operator = (BlockInterval& second);
};

#endif // BLOCKINTERVAL_H
