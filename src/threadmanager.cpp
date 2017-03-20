#include <QCryptographicHash>
#include <QVector>
#include <iostream>
#include "threadmanager.h"
#include "threadhack.h"
/*
 * std::pow pour les long long unsigned int
 */
long long unsigned int intPow (
        long long unsigned int number,
        long long unsigned int index)
{
    long long unsigned int i;

    if (index == 0)
        return 1;

    long long unsigned int num = number;

    for (i = 1; i < index; i++)
        number *= num;

    return number;
}

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{}
/*
 * Les paramètres sont les suivants:
 *
 * - charset:   QString contenant tous les caractères possibles du mot de passe
 * - salt:      le sel à concaténer au début du mot de passe avant de le hasher
 * - hash:      le hash dont on doit retrouver la préimage
 * - nbChars:   le nombre de caractères du mot de passe à bruteforcer
 * - nbThreads: le nombre de threads à lancer
 *
 * Cette fonction doit retourner le mot de passe correspondant au hash, ou une
 * chaine vide si non trouvé.
 */
QString ThreadManager::startHacking(
        QString charset,
        QString salt,
        QString hash,
        unsigned int nbChars,
        unsigned int nbThreads
)
{
    //initialisation des variables statiques
    ThreadHack::finished = false;
    ThreadHack::password = "";

    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    unsigned int nbValidChars;

    /*
     * Mot de passe à tester courant
     */
    QString currentPasswordString;

    /*
     * Tableau contenant les index dans la chaine charset des caractères de
     * currentPasswordString
     */
    QVector<unsigned int> currentPasswordArray;

    /*
     * Hash du mot de passe à tester courant
     */
    QString currentHash;

    /*
     * Calcul du nombre de hash à générer
     */
    nbToCompute = intPow(charset.length(),nbChars);
    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    nbValidChars = charset.length();

    //nombre d'itérations que devront faire chaques threads.
    long long unsigned int nbComputeByThread = nbToCompute/nbThreads;
    //initialisation variable d'indication de l'itération de départ du thread.
    long long unsigned int startAt = 0;

    //déclaration d'un thread.
    ThreadHack* myThread;

    //création des threads, stockage dans une liste et connexion des signaux.
    for(unsigned int i = 0; i < nbThreads; i++)
    {
        /* Il peux arriver que la division du nombre total d'itérations a effectuer
        ne tombe pas juste, il s'agit donc ici, lors du dernier thread, de lui passer
        le reste de la division entière.
        */
        if(i+1 == nbThreads){
            nbComputeByThread = nbToCompute - startAt;
        }
        //initialisation des threads.
        myThread = new ThreadHack(charset,nbComputeByThread, nbValidChars, salt, nbChars, hash, startAt);

        //ajout du thread a la liste des threads.
        threadList.append(myThread);

        //connexion des signaux.
        connect(myThread, SIGNAL(signalProg()), this, SLOT(progressionThread()));

        //démarrage du thread.
        myThread->start();
        //ajout du nombre d'itérations effectuer par le thread pour mettre a jours
        // le début du suivant.
        startAt += nbComputeByThread;

    }

    //Boucle pour l'attente sur les threads et la suppression.
    for(unsigned int i = 0; i < nbThreads; i++)
    {
        //récupération du thread a l'indice i.
        myThread = threadList.at(i);
        //attente de la terminaison du thread.
        myThread->wait();
        //liberation de la mémoire.
        delete myThread;
    }
    //clear de la liste de thread.
    threadList.clear();

    /*
     * Si on arrive ici, cela signifie que tous les mot de passe possibles ont
     * été testés, et qu'aucun n'est la préimage de ce hash.
     */
    return ThreadHack::password;
}
//fonction d'emision du signal pour la barre de progression.
void ThreadManager::progressionThread()
{
    emit incrementPercentComputed((double)1000/nbToCompute);
}
