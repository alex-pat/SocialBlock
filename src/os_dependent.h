#ifndef OS_DEPENDENT_H
#define OS_DEPENDENT_H

#if defined(Q_OS_LINUX)
const QString HOSTS = "/etc/hosts";
#elif defined(Q_OS_WIN)
const QString HOSTS = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#elif defined(Q_OS_OSX)
const QString HOSTS = "/private/etc/hosts";
#endif

#endif // OS_DEPENDENT_H
