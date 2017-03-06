#include "threadhack.h"

ThreadHack::ThreadHack(unsigned int nb, unsigned int sh):nbThreads(nb), shift(sh)
{}

void ThreadHack::run()
{
    currentPasswordString.fill(charset.at(0),nbChars);
    currentPasswordArray.fill(0,nbChars);

    while (nbComputed < nbToCompute)
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
            return currentPasswordString;

        /*
         * Tous les 1000 hash calculés, on notifie qui veut bien entendre
         * de l'état de notre avancement (pour la barre de progression)
         */
        if (!(nbComputed % 1000))
            emit incrementPercentComputed((double)1000/nbToCompute);

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

        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i] += nbThreads;

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
        for (i=0;i<nbChars;i++)
            currentPasswordString[i]  = charset.at(currentPasswordArray.at(i));

        nbComputed++;
    }
}
