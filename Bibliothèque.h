/**
 * Project Untitled
 */

#ifndef _BIBLIOTHÈQUE_H
#define _BIBLIOTHÈQUE_H

class Bibliothèque
{
public:
    void créerBibliotheque();

    void modifierBibliotheque();

    void supprimerBibliotheque();

    void chargerBibliotheque();

    void sauvegarderBibliotheque();

    double calculerCoutMin();

    double calculerCoutMax();

    double calculerCoutMoyen();

    void filter();

    void ajouterDescripteur();

    void enleveDescripteur();

    void trierImage();

    /**
     * @param cout
     */
    void chercherImage(void cout);

private:
    void ID;
    String _nom;
    Descripteur ListeDescripteurs;
};

#endif //_BIBLIOTHÈQUE_H