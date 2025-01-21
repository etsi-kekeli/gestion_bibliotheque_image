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
    Bibliotheque(const string &nom, vector<Descripteur*>* descripteurs);
    ~Bibliotheque();

    // Getters and Setters
    const string &getNom() const;
    void setNom(const string nom);

    vector<Descripteur*>* getDescripteurs() const;
    void setImages(vector<Descripteur*>* descripteurs);

    void ajouterDescripteur(Descripteur *nouveauDescripteur);
    void enleveDescripteur(string &source);

    // const vector<Utilisateur> &getUtilisateurs() const;
    // void setUtilisateurs(const vector<Utilisateur> &utilisateurs);

    // les méthodes
    // void creerBibliotheque();

    // void modifierBibliotheque();

    void supprimerBibliotheque();

    bool chargerBibliotheque(string nomDuFichier);

    bool sauvegarderBibliotheque(string nomDuFichier) const;

    double calculerCoutMin();

    double calculerCoutMax();

    double calculerCoutMoyen();

    int nombreDImagesLibres();

    vector<Descripteur*>* filter(double coutMin = 0, double coutMax = numeric_limits<float>::max());

    vector<Descripteur*>* trierDescripteurs();

    /**
     * @param cout
     */
    double chercherCoutImage(int ID);

private:
    // void ID; rappelez moi pourquoi ID pour la bibliotheque ?
    string nom;
    vector<Descripteur*> *descripteurs;
    const static int signature;
    // vector<Utilisateur> utilisateurs;
};

#endif //_BIBLIOTHEQUE_H
