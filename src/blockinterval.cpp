#include "blockinterval.h"

BlockInterval::BlockInterval() :
    timeBegin (  8, 0),
    timeEnd   ( 12, 0)
{
    adresses << "vk.com"
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
    timeBegin.setHMS(hh, mm);
}

void BlockInterval::setEndTime(int hh, int mm) {
    timeEnd.setHMS(hh, mm);
}

bool BlockInterval::isIncludeTime(QTime &time) {
    QTime current = QTime::currentTime();
    if ( current < timeBegin )
        return false;
    if ( current > timeEnd )
        return false;
    return true;
}
