#ifndef _DESCRIPTEUR_H
#define _DESCRIPTEUR_H

#include <string>
#include <set>
#include "Image.h"

enum class Acces : char
{
    OUVERT = 'O',
    LIMITE = 'L'
};
class Descripteur
{
public:
    // Constructeurs
    Descripteur();
    Descripteur(int id, const std::string &titre, std::string &source, double cout, const std::string &auteur);
    // Destructeur
    ~Descripteur();

    // Getters and Setters
    int getIdDescripteur() const;
    void setIdDescripteur(int id);

    const std::string &getTitre() const;
    void setTitre(const std::string &titre);

    const std::string &getSource() const;

    double getCout() const;
    void setCout(double cout);

    const std::string &getAuteur() const;
    void setAuteur(const std::string &auteur);

    Acces getAcces() const;
    void setAcces(Acces acces);

    // Méthodes
    void modifierDescripteur();

    void supprimerDescripteur();

    void creerDescripteur(const Image &image);

private:
    int IdDescripteur;
    std::string Titre;
    std::string source;
    double cout;
    std::string Auteur;
    Acces acces;
    static std::set<std::string> sourcesUtilisees; // Pour vérifier l'unicité de la source
};

bool comparerParCout(Descripteur &d1, Descripteur &d2);

#endif //_DESCRIPTEUR_H