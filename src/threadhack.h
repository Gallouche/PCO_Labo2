#ifndef THREADHACK_H
#define THREADHACK_H
#include <QVector>
#include <QCryptographicHash>

class ThreadHack :public QThread
{
private:
    unsigned int nbThreads;
    unsigned int shift;
    QString currentPasswordString;
    QVector currentPasswordArray;

public:
    void run();
    ThreadHack(unsigned int nb, unsigned int sh);
};

#endif // THREADHACK_H
