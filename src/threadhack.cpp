#include "threadhack.h"

bool ThreadHack::finished;
QString ThreadHack::password;

ThreadHack::ThreadHack(QString ch,
                       long long unsigned int nbToCom,
                       unsigned int nbVCh,
                       QString s,
                       unsigned int nbC,
                       QString h,
                       long long unsigned int star)
    : charset(ch), nbToCompute(nbToCom), nbValidChars(nbVCh), salt(s), nbChars(nbC), hash(h), startAt(star)
{}

void ThreadHack::run()
{
    unsigned int i;
    /*
     * Object QCryptographicHash servant à générer des md5
     */
    QCryptographicHash md5(QCryptographicHash::Md5);

    /*
     * On initialise le premier mot de passe à tester courant en le remplissant
     * de nbChars fois du premir carcatère de charset
     */
    currentPasswordString.fill(charset.at(0),nbChars);
    currentPasswordArray.fill(0,nbChars);

    long long unsigned int nbComputed = 0; // initialisation du nombre computed à 0 (nombre de mots-de-passe traités)

    /*
     * On calcul les valeurs auxquelles currentPasswordArray doit être initialisé, différents pour chaque thread pour
     * que les threads ne commence pas par les mêmes mots-de-passes. Point de départ de chaqu'un des threads.
     */
    for (unsigned int j = 0 ; j < nbChars; j++)
    {
        currentPasswordArray[j] = startAt % charset.length();
        startAt /= charset.length();
    }
   /*
    *Tant que le thread n'a pas fini de tester tout les mots-de-passes qui lui sont attribués,
    *et que le mot-de-passe n'a pas encore été trouvé (par tous les threads) nous continuous de chercher
    */
    while (nbComputed <= nbToCompute && !finished)
    {
        /* On vide les données déjà ajoutées au générateur */
        md5.reset();

        /* On préfixe le mot de passe avec le sel */
        md5.addData(salt.toLatin1());
        md5.addData(currentPasswordString.toLatin1());

        /* On calcul le hash */
        currentHash = md5.result().toHex();

        /*
         * Si on a trouvé, on retourne le mot de passe courant (sans le sel)
         */
        if (currentHash == hash)
        {
            password = currentPasswordString;
            finished = true;
            return;
        }

        /*
         * Tous les 1000 hash calculés, on notifie qui veut bien entendre
         * avec signalProg()
         *
         * Ce signal va être récupéré dans threadManager.cpp
         * qui va ensuite emit pour la progression de la barre de progression
         */
        if (!(nbComputed % 1000))
        {
            emit signalProg();
        }
        /*
         * On récupère le mot de pass à tester suivant.
         *
         * L'opération se résume à incrémenter currentPasswordArra" comme si
         * chaque élément de ce vecteur représentait un digit d'un nombre en
         * base nbValidChars.
         *
         * Le digit de poids faible étant en position 0
         */
        i = 0;
        while (i < nbChars)
        {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= nbValidChars) {
                currentPasswordArray[i] = 0;  
                i++;
            } else
                break;
        }

        /*
         * On traduit les index présents dans currentPasswordArray en
         * caractères
         */

        for (unsigned int i = 0; i<nbChars; i++)
        {
           currentPasswordString[i]  = charset.at(currentPasswordArray.at(i));
        }
        // On incremente le nombre de hash testé
        nbComputed++;
    }
}
