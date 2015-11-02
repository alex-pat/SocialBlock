#ifndef BLOCKINTERVAL_H
#define BLOCKINTERVAL_H

#include <QTime>

class BlockInterval
{
    QTime timeBegin;
    QTime timeEnd;
    QList <QString> adresses;
public:
    BlockInterval();
};

#endif // BLOCKINTERVAL_H
