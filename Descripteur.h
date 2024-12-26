#ifndef _DESCRIPTEUR_H
#define _DESCRIPTEUR_H

#include <string>
#include <set>
#include "Image.h"

class Descripteur
{
public:
    // Constructeurs
    Descripteur();
    Descripteur(int id, const std::string& titre, std::string& source, double cout, const std::string& auteur);
    // Destructeur
    ~Descripteur();

    // Getters and Setters
    int getIdDescripteur() const;
    void setIdDescripteur(int id);

    const std::string& getTitre() const;
    void setTitre(const std::string& titre);

    const std::string& getSource() const;

    double getCout() const;
    void setCout(double cout);

    const std::string& getAuteur() const;
    void setAuteur(const std::string& auteur);


    void TrierImage();

    /**
     * @param cout
     */
    void ChercherImage(double cout);

    void FilterImage();

    void modifierDescripteur();

    void supprimerDescripteur();

    void creerDescripteur(const Image& image);

private:
    int IdDescripteur;
    std::string Titre;
    std::string source;
    double cout;
    std::string Auteur;
    static std::set<std::string> sourcesUtilisees; // Pour vérifier l'unicité de la source
};

#endif //_DESCRIPTEUR_H