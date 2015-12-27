#ifndef OS_DEPENDENT_H
#define OS_DEPENDENT_H

#if defined(Q_OS_LINUX)
#define HOSTS "/etc/hosts"
#elif defined(Q_OS_WIN)
#define HOSTS "C:\Windows\System32\drivers\etc\hosts"
#elif defined(Q_OS_OSX)
#define HOSTS "/private/etc/hosts"
#endif

#endif // OS_DEPENDENT_H

