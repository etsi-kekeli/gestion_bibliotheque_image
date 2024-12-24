/**
 * Project Untitled
 */

#ifndef _DESCRIPTEUR_H
#define _DESCRIPTEUR_H

#include <string>


class Descripteur
{
public:
    // Constructeurs
    Descripteur();
    Descripteur(int id, const std::string& titre, const std::string& source, double cout, const std::string& auteur);
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

    void CréerDescripteur();

    void ModifierDescripteur();

    void getters();

    void setters();

    void modifierDescripteur();

    void supprimerDescripteur();

    void creerDescripteur();

private:
    int IdDescripteur;
    std::string Titre;
    const std::string source;
    double cout;
     std::string Auteur;
};

#endif //_DESCRIPTEUR_H