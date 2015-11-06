#include "profile.h"

Profile::Profile() :
    name ("Untitled")
{

}

void Profile::addInterval(int day, BlockInterval *newInterv) {
    week[day].push_back(newInterv);
}

void Profile::deleteInterval(int day, int index) {
    // TODO !!!!111!!!!1
}

void Profile::changeInterval(int day, int index, BlockInterval *) {
    // TODO !!!!111!!!!1
}

void Profile::addToHosts(QString &strHosts) {
    QDate currentDate = QDate::currentDate();
    int day = currentDate.dayOfWeek() - 1;
    QStringList listHosts = strHosts.split('\n');
    QStringListIterator iter = listHosts.begin(),
                        end = listHosts.end();
    QListIterator urlIter ( week[day] );


    while ( iter.hasNext() ) {

    }
}
