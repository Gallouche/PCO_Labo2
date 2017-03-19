#ifndef THREADHACK_H
#define THREADHACK_H
#include <QVector>
#include <QCryptographicHash>
#include <QThread>
#include <QString>
#include <QObject>


class ThreadHack : public QThread
{
    Q_OBJECT
protected:
    void run();
private:
    unsigned int nbThreads;
    unsigned int shift;
    QString currentPasswordString;
    QVector<unsigned int> currentPasswordArray;
    QString charset;
    unsigned int nbValidChars;
    long long unsigned int nbToCompute;
    QString salt;
    unsigned int nbChars;
    QString currentHash;
    QString hash;
    int indexTh;
    long long unsigned int startAt;
public:
    static QString password;
    static bool finished;
    ThreadHack(QString ch,
               long long unsigned int nbToCom,
               unsigned int nbVCh,
               QString s,
               unsigned int nbC,
               QString h,
               long long unsigned int star, int inde);
signals:
    void signalProg();
};

#endif // THREADHACK_H
