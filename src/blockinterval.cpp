#include "blockinterval.h"

BlockInterval::BlockInterval() :
    timeBegin (  8, 0),
    timeEnd   ( 12, 0)
{
    addresses << "vk.com"
             << "facebook.com"
             << "twitter.com"
             << "instagram.com"
             << "instagr.am"
             << "youtube.com"
             << "ok.ru"
             << "surfinbird.ru"
             << "4pda.ru"
             << "onliner.by"
             << "tut.by"
             << "opennet.ru"
             << "postnauka.ru"
             << "zoom.cnews.ru";
}

void BlockInterval::setBeginTime(int hh, int mm) {
    timeBegin.setHMS(hh, mm, 0);
}

void BlockInterval::setEndTime(int hh, int mm) {
    timeEnd.setHMS(hh, mm, 0);
}

bool BlockInterval::isIncludeTime( bool now ) {
    QTime current = QTime::currentTime();
    if ( !now )
        current = current.addSecs(-60);
    if ( current < timeBegin )
        return false;
    if ( current > timeEnd )
        return false;
    return true;
}

void BlockInterval::setNewAdresses(QStringList &newAddr) {
    addresses = newAddr;
}

QStringList& BlockInterval::getAddresses () {
    return addresses;
}

BlockInterval& BlockInterval::operator = (BlockInterval& second) {
    timeBegin = second.timeBegin;
    timeEnd = second.timeEnd;
    addresses = second.addresses;
    return *this;
}

QDataStream& operator<< (QDataStream& stream, BlockInterval& interv) {
    stream << interv.timeBegin
           << interv.timeEnd;
    int addrSize = interv.addresses.size();
    stream << addrSize;
    for (int i = 0; i < addrSize; i++)
        stream << interv.addresses[i];
    return stream;
}

QDataStream& operator>> (QDataStream& stream, BlockInterval* interv) {
    stream >> interv->timeBegin
           >> interv->timeEnd;
    int addrSize;
    stream >> addrSize;
    interv->addresses.clear();
    for (int i = 0; i < addrSize; i++) {
        QString tempString;
        stream >> tempString;
        interv->addresses.push_back(tempString);
    }
    return stream;
}

QString BlockInterval::getTime() {
    return ( timeBegin.toString("HH:mm")
             + " - " +
             timeEnd.toString("HH:mm") );
}

QString BlockInterval::getSitesString() {
    return addresses.join(' ');
}
