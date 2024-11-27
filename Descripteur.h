/**
 * Project Untitled
 */

#ifndef _DESCRIPTEUR_H
#define _DESCRIPTEUR_H

using namespace std;

class Descripteur
{
public:
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
    string Titre;
    const string source;
    double cout;
    string Auteur;
};

#endif //_DESCRIPTEUR_H