#include "threadhack.h"
#include <iostream>
bool ThreadHack::finished;
QString ThreadHack::password;

ThreadHack::ThreadHack(QString ch,
                       long long unsigned int nbToCom,
                       unsigned int nbVCh,
                       QString s,
                       unsigned int nbC,
                       QString h,
                       long long unsigned int star, int inde)
    : charset(ch), nbToCompute(nbToCom), nbValidChars(nbVCh), salt(s), nbChars(nbC), hash(h), startAt(star), indexTh(inde)
{}

void ThreadHack::run()
{
     int idx = 0;
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

    long long unsigned int nbComputed = 0;
    for (int j = currentPasswordArray.length()-1; j >= 0; j--)
    {
        if(indexTh == 1)
            //std::cout <<"c est la-----------" <<currentPasswordArray.length() -1 -j << std::endl;
        currentPasswordArray[currentPasswordArray.length() -1 -j] = startAt % charset.length();
        startAt /= charset.length();

        //std::cout << currentPasswordArray[j];
    }
    //std::cout <<nbToCompute<< std::endl;
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
         * de l'état de notre avancement (pour la barre de progression)
         */
        if (!(nbComputed % 1000))
        {
           //std::cout <<"star : " << startAt <<currentPasswordString.toStdString() << std::endl;
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
        while (i < (int)currentPasswordArray.size()) {
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

        for (unsigned int i = 0;i<nbChars;i++)
            currentPasswordString[i]  = charset.at(currentPasswordArray.at(i));
       if(indexTh == 1)
            //std::cout <<"---- "<< indexTh<<"   "<< currentPasswordString.toStdString() << "--------- "<< idx << std::endl;
        idx++;
        nbComputed++;
    }
}
