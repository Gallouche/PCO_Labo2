/**
  \file threadmanager.h
  \author Yann Thoma
  \date 24.02.2017
  \brief Classe pour reverser un hash md5 par brute force.


  Ce fichier contient la définition de la classe ThreadManager, qui permet de
  reverser un hash md5 par brute force.
*/

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QString>
#include "threadhack.h"
/**
 * \brief The ThreadManager class
 *
 * Cette classe tente de reverser un hash md5 par brute force.
 */
class ThreadManager: public QObject
{
    Q_OBJECT
private:
    //Liste pour stocker les threads
    QList<ThreadHack*> threadList;
    //nombre total d'itérations.
    long long unsigned int nbToCompute;
public:
    /**
     * \brief ThreadManager Constructeur simple
     * \param parent Objet parent de l'interface
     */
    ThreadManager(QObject *parent);

    /**
     * \brief startHacking tâche qui s'occupe d'attaquer le hash md5.
     * \param charset QString tous les caractères possibles composant le mot de
     * passe
     * \param salt QString sel qui permet de modifier dynamiquement le hash
     * \param hash QString hash à reverser
     * \param nbChars taille du mot de passe
     * \param nbThreads nombre de threads qui doivent reverser le hash
     */
    QString startHacking(
            QString charset,
            QString salt,
            QString hash,
            unsigned int nbChars,
            unsigned int nbThreads
    );

signals:
    /**
     * \brief incrementPercentComputed fonction qui indique que le pourcentage
     * de test effectué pour casser le hash md5.
     * \param percentComputed double pourcentage de tests effectués pour
     * reverser le hash md5
     */
    void incrementPercentComputed(double percentComputed);
public slots:
    //fonction appellée par le signal envoyer par le thread
    void progressionThread();
};

#endif // THREADMANAGER_H
