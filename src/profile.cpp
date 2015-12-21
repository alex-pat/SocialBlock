#include "profile.h"
#include <QFile>
#include <iostream>

Profile::Profile() :
    name ("Untitled")
{
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 10; i++) {
            BlockInterval *interv = new BlockInterval;
            interv->setBeginTime(i,j);
            week[j].push_back( interv );
        }

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
    QFile hosts ("/etc/hosts");
    if ( hosts.open( QIODevice::WriteOnly |
                     QIODevice::Text |
                     QIODevice::Append ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        exit(1);
    }

    int today = QDate::currentDate().dayOfWeek() - 1;

    //current list search
    QListIterator <BlockInterval* > intervIter (week[today]);
    while ( intervIter.hasNext() &&
            !( intervIter.next()->isIncludeTime() ) )
        ;

    if ( ! intervIter.hasNext() )
        return;

    QListIterator <QString> site (intervIter.peekPrevious()->getAddresses());
    while ( site.hasNext() ) {
        QString str= "127.0.0.1\t" + site.peekNext() + "\t# Blocked by SocialBlock\n"
                  "127.0.0.1\twww." + site.next() + "\t# Blocked by SocialBlock\n";
        hosts.write( str.toStdString().c_str() );
    }

    hosts.close();
}

void Profile::removeFromHosts() {
    int today = QDate::currentDate().dayOfWeek() - 1;

    //current list search
    QListIterator <BlockInterval* > intervIter (week[today]);
    while ( intervIter.hasNext() &&
            !( intervIter.next()->isIncludeTime() ) )
        ;

    if ( ! intervIter.hasNext() )
        return;

    QStringListIterator sites (intervIter.peekPrevious()->getAddresses() );

    QFile hosts ("/etc/hosts");
    if ( hosts.open( QIODevice::ReadOnly |
                     QIODevice::Text ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        return;
    }

    QStringList listHosts = QString( hosts.readAll().toStdString().c_str() ).split('\n');
    hosts.close();

    QMutableStringListIterator iterHosts(listHosts);

    while ( iterHosts.hasNext() ) {
        iterHosts.next();
        sites.toFront();
        while ( sites.hasNext() )
            if ( iterHosts.peekPrevious().contains(sites.peekNext()) ) {
                iterHosts.remove();
                break;
            }
        if ( !sites.hasNext() )
            iterHosts.previous();
    }

    if ( hosts.open( QIODevice::WriteOnly |
                     QIODevice::Text ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        return;
    }

    hosts.write ( listHosts.join('\n').toStdString().c_str() );

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
            stream >> prof->week[i][j];
        }
    }
    return stream;
}

QStringList Profile::getIntervals(int day) const {
    QStringList result;
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
