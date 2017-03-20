#ifndef THREADHACK_H
#define THREADHACK_H
#include <QVector>
#include <QCryptographicHash>
#include <QThread>
#include <QString>
#include <QObject>

/**
 * @brief The ThreadHack class : Classe qui hérite de QThread, va être la classe inctanciée pour nos threads qui recherchent les mot-de-passes
 */
class ThreadHack : public QThread
{
    Q_OBJECT
protected:

    void run();

private:

    unsigned int nbThreads;
    unsigned int shift;
    unsigned int nbValidChars;
    unsigned int nbChars;
    long long unsigned int startAt;
    long long unsigned int nbToCompute;

    QVector<unsigned int> currentPasswordArray;
    QString salt;
    QString hash;
    QString charset;
    QString currentHash;
    QString currentPasswordString;

public:

    static QString password; //String à remplir avec le contenu du mot de passe trouvé, par n'importe lequel des threads
    static bool finished;    //Booleen static qui passe a true une fois le mot-de-passe trouvé. utile pour arreter les autres threads
    /**
     * @brief ThreadHack : Constructeur de la classe ThreadHack. C'est notre fonction qui hérite de QThread
     * @param ch         : QString contenant les 66 char possibles pour construire les mots de passe : a..z, A..Z, 0..9, et 4 autres carac. spéciaux
     * @param nbToCom    : Entier contenant le nombre de mots-de-passe à tester ( au maximum biensur) pour chacun des threads
     * @param nbVCh      : Entier qui représente le nombre de car contenus dans le charset (66 actuellemtn)
     * @param s          : Valeur du sel du mot-de-passe
     * @param nbC        : Entier qui correspond au nombre de caractères du mot-de-passe (entrée par l'utilisateur)
     * @param h          : QString contenant le Hash du mot-de-passe à retrouver. Ce hash est entrée par l'utilisateur
     * @param star       : Entier représentant la position à laquelle le thread construit va commencer à tester les hash.
     *                     (par ex : si i est envoyé, le thread va commencer au ième mot-de-passe possible.
     */
    ThreadHack(QString ch,
               long long unsigned int nbToCom,
               unsigned int nbVCh,
               QString s,
               unsigned int nbC,
               QString h,
               long long unsigned int star);

signals:
    //signal envoyé de chaque thread vers le threadmanager, toute les 1000 itérations, qui informe qu'il à avancé de mille itérations
    void signalProg();
};

#endif // THREADHACK_H
