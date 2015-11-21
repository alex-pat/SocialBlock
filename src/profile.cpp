#include "profile.h"
#include <QFile>
#include <iostream>

Profile::Profile() :
    name ("Untitled")
{

}

void Profile::addInterval(int day, BlockInterval *newInterv) {
    week[day].push_back(newInterv);
}

void Profile::deleteInterval(int day, int index) {
    delete week[day].at(index);
    week[day].removeAt(index);
}

void Profile::changeInterval(int day, int index, BlockInterval *newInterv) {
    delete week[day].at(index);
    week[day].at(index) = newInterv;
}

void Profile::writeToHosts() {
    QFile hosts ("/etc/hosts");
    if ( hosts.open( QIODevice::WriteOnly |
                     QIODevice::Text |
                     QIODevice::Append ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        return;
    }

    int today = QDate::currentDate().dayOfWeek() - 1;

    //current list search
    QListIterator <BlockInterval* > intervIter (week[today]);
    while ( intervIter.hasNext() &&
            !( intervIter.next()->isIncludeTime() ) )
        ;

    if ( ! intervIter.hasNext() )
        return;

    QListIterator <QString> site (intervIter.peekPrevious());
    while ( site.hasNext() ) {
        std::string str= "127.0.0.1\t" + site.peekNext() + "\t# Blocked by SocialBlock\n"
                  "127.0.0.1\twww." + site.next() + "\t# Blocked by SocialBlock\n";
        hosts.write( str );
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

    QListIterator <QString> sites (intervIter.peekPrevious()->getAddresses() );

    QFile hosts ("/etc/hosts");
    if ( hosts.open( QIODevice::ReadOnly |
                     QIODevice::Text ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        return;
    }

    QStringList listHosts = QString( hosts.readAll().toStdString().c_str() ).split('\n');
    hosts.close();

    QStringList::iterator iterHosts, hostsSize = listHosts.size();

    for ( iterHosts = 0; iterHosts < hostsSize; ++iterHosts) {
        sites.toFront();
        while ( sites.hasNext() )
            if ( listHosts.at(iterHosts).find( sites.next() ) != std::string::npos ) {
                listHosts.removeAt( iterHosts-- );
                break;
            }
    }

    if ( hosts.open( QIODevice::WriteOnly |
                     QIODevice::Text ) == false ) {
        std::cerr << "Cannot open /etc/hosts" << std::endl;
        return;
    }

    hosts.write ( listHosts.join('\n').toStdString().c_str() );

}
