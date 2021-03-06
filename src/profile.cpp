#include "profile.h"
#include "os_dependent.h"
#include <QFile>
#include <QMessageBox>
#include <iostream>

Profile::Profile() :
    name ("Untitled")
{ }

Profile::~Profile() {
    for (int i = 0; i < 7; i++) {
        int size = week[i].size();
        for (int j = 0; j < size; j++)
            delete week[i].at(j);
    }
}

void Profile::addInterval(int day, BlockInterval *newInterv) {
    week[day].push_back(newInterv);
}

void Profile::deleteInterval(int day, int index) {
    delete week[day].at(index);
    week[day].removeAt(index);
}

void Profile::changeInterval(int day, int index, BlockInterval &newInterv) {
    delete week[day].at(index);
    *( week[day].at(index) ) = newInterv;
}

void Profile::setName(QString newName) {
    name = newName;
}

void Profile::writeToHosts() {
    QFile hosts (HOSTS);
    if ( hosts.open( QIODevice::WriteOnly |
                     QIODevice::Text |
                     QIODevice::Append ) == false ) {
        std::cerr << "Cannot open hosts file" << std::endl;
        QMessageBox::critical(NULL, "Error", "Cannot open hosts file, try open app with admin permissions");
        exit(1);
    }

    int today = QDate::currentDate().dayOfWeek() - 1;

    //current list search
    QListIterator <BlockInterval* > intervIter (week[today]);
    while ( intervIter.hasNext() &&
            !( intervIter.peekNext()->isIncludeTime() ) )
        intervIter.next();

    if ( ! intervIter.hasNext() )
        return;
    QListIterator <QString> site (intervIter.peekNext()->getAddresses());

    //writing strings to hosts
    while ( site.hasNext() ) {
        QString str = "127.0.0.1\t" + site.peekNext() + "\t# Blocked by SocialBlock\n"
                  "127.0.0.1\twww." + site.peekNext() + "\t# Blocked by SocialBlock\n";
        hosts.write( str.toStdString().c_str() );
        site.next();
    }

    hosts.close();
}

void Profile::removeFromHosts( bool isNow ) {
    QFile hosts (HOSTS);
    if ( hosts.exists() == false )
        return;

    int today = QDate::currentDate().dayOfWeek() - 1;

    //current list search
    QListIterator <BlockInterval* > intervIter (week[today]);
    while ( intervIter.hasNext() &&
            !( intervIter.peekNext()->isIncludeTime( isNow ) ) )
        intervIter.next();

    // ejecting data from hosts
    if ( intervIter.hasNext() == false )
        return;

    if ( hosts.open( QIODevice::ReadWrite |
                     QIODevice::Text ) == false ) {
        std::cerr << "Cannot open hosts file" << std::endl;
        QMessageBox::critical(NULL, "Error", "Cannot open hosts file, try open app with admin permissions");
        return;
    }

    QStringList listHosts = QString( hosts.readAll().toStdString().c_str() ).split('\n');

    //deleting strings from list
    QMutableStringListIterator iterHosts(listHosts);

    QStringListIterator sites (intervIter.peekNext()->getAddresses() );

    while ( sites.hasNext() ) {
        iterHosts.toFront();
        do {
            iterHosts.next();
            if ( iterHosts.peekPrevious().contains( sites.peekNext() ) )
                iterHosts.remove();
        } while ( iterHosts.hasNext() );
        sites.next();
    }

    //writings changes
    hosts.resize(0);
    hosts.write ( listHosts.join('\n').toStdString().c_str() );
    hosts.close();
}

QString Profile::getName() const {
    return name;
}

QDataStream& operator<< (QDataStream& stream, Profile& prof) {
    stream << prof.name;
    for (int i = 0; i < 7; i++) {
        int intervCount = prof.week[i].size();
        stream << intervCount;
        for (int j = 0; j < intervCount; j++)
            stream << *(prof.week[i][j]);
    }
    return stream;
}

QDataStream& operator>> (QDataStream& stream, Profile* prof) {
    stream >> prof->name;
    for (int i = 0; i < 7; i++) {
        int intervCount;
        stream >> intervCount;
        for (int j = 0; j < intervCount; j++) {
            BlockInterval* interval = new BlockInterval;
            stream >> interval;
            prof->week[i].push_back(interval);
        }
    }
    return stream;
}

QStringList Profile::getIntervals(int day) {
    QStringList result;
    if ( day < 0 || day > 6 )
        return result;
    QListIterator<BlockInterval*> iter (week[day]);
    while (iter.hasNext())
        result << iter.next()->getTime();
    return result;
}

QStringList Profile::getSites(int day) const {
    QStringList result;
    QListIterator<BlockInterval*> iter (week[day]);
    while (iter.hasNext())
        result << ( iter.next()->getSitesString() );
    return result;
}

bool Profile::isBlockedNow() const {
    QListIterator<BlockInterval*> iter (week[QDate::currentDate().dayOfWeek() - 1]);
    while ( iter.hasNext() )
        if ( iter.next()->isIncludeTime() )
            return true;
    return false;
}

void Profile::fillStandartList(QStringList &urlList) {
    for (int i = 0; i < 7; i++) {
        BlockInterval* interv = new BlockInterval;
        interv->setBeginTime( 9, 0);
        interv->setEndTime  (12, 0);
        interv->setNewAdresses (urlList);
        addInterval(i, interv);
    }
}
