/**
 * Project Untitled
 */

#ifndef _BIBLIOTHÈQUE_H
#define _BIBLIOTHÈQUE_H

#include <string>
#include <vector>
#include "Descripteur.h"
#include "Utilisateur.h"

class Bibliotheque
{
public:
    // Constructeur et Destructeur
    Bibliotheque();
    Bibliotheque(const std::string& nom, const std::vector<Descripteur>& descripteurs, const std::vector<Utilisateur>& utilisateurs);
    ~Bibliotheque();

     // Getters and Setters
    const std::string& getNom() const;
    void setNom(const std::string& nom);

    const std::vector<Descripteur>& getDescripteurs() const;
    void setImages(const std::vector<Descripteur>& descripteurs);

    const std::vector<Utilisateur>& getUtilisateurs() const;
    void setUtilisateurs(const std::vector<Utilisateur>& utilisateurs);


    void creerBibliotheque();

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
    void chercherImage(double cout);

private:
    // void ID; rapelez moi pourquoi ID pour la bibliotheque ? 
    std::string nom;
    std::vector<Descripteur> descripteurs;
    std::vector<Utilisateur> utilisateurs;
};

#endif //_BIBLIOTHÈQUE_H