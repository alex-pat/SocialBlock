#ifndef BLOCKINTERVAL_H
#define BLOCKINTERVAL_H

#include <QTime>
#include <QDataStream>

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
    QString getTime ();
    QString getSitesString();
    QStringList& getAddresses();
    BlockInterval& operator = (BlockInterval& second);

    friend QDataStream& operator<< (QDataStream& stream, BlockInterval& interv);
    friend QDataStream& operator>> (QDataStream& stream, BlockInterval* interv);

};

#endif // BLOCKINTERVAL_H
