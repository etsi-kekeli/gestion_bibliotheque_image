#ifndef _BIBLIOTHEQUE_H
#define _BIBLIOTHEQUE_H

#include <string>
#include <vector>
#include "Descripteur.h"
#include "Utilisateur.h"

using namespace std;

class Bibliotheque
{
public:
    // Constructeur et Destructeur
    Bibliotheque();
    Bibliotheque(const string &nom, const vector<Descripteur> &descripteurs);
    // ~Bibliotheque();

    // Getters and Setters
    const string &getNom() const;
    void setNom(const string &nom);

    const vector<Descripteur> &getDescripteurs() const;
    void setImages(const vector<Descripteur> &descripteurs);

    void ajouterDescripteur(Descripteur &nouveauDescripteur);
    void enleveDescripteur(string &source);

    // const vector<Utilisateur> &getUtilisateurs() const;
    // void setUtilisateurs(const vector<Utilisateur> &utilisateurs);

    // les méthodes
    // void creerBibliotheque();

    // void modifierBibliotheque();

    void supprimerBibliotheque();

    void chargerBibliotheque();

    void sauvegarderBibliotheque();

    double calculerCoutMin();

    double calculerCoutMax();

    double calculerCoutMoyen();

    vector<Descripteur> filter();

    vector<Descripteur> trierDescripteurs();

    /**
     * @param cout
     */
    void chercherImage(double cout);

private:
    // void ID; rappelez moi pourquoi ID pour la bibliotheque ?
    string nom;
    vector<Descripteur> descripteurs;
    // vector<Utilisateur> utilisateurs;
};

#endif //_BIBLIOTHEQUE_H