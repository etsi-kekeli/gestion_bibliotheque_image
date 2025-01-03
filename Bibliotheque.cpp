#include "Bibliotheque.h"
#include <algorithm>
using namespace std;

/**
 * Bibliothèque implementation
 */
// Constructeur par défaut
Bibliotheque::Bibliotheque()
{
    nom = "bibliothèque";
}

// Constructeur paramétré
Bibliotheque::Bibliotheque(const string &nom, const vector<Descripteur> &descripteurs)
    : nom(nom), descripteurs(descripteurs)
{
}
// Destructeur
// Bibliotheque::~Bibliotheque()
// {
// }

// Getters and Setters
const string &Bibliotheque::getNom() const
{
    return nom;
}

void Bibliotheque::setNom(const string &nom)
{
    this->nom = nom;
}

const vector<Descripteur> &Bibliotheque::getDescripteurs() const
{
    return descripteurs;
}

void Bibliotheque::setImages(const vector<Descripteur> &descripteurs)
{
    this->descripteurs = descripteurs;
}

void Bibliotheque::ajouterDescripteur(Descripteur &nouveauDescripteur)
{
    for (int i = 0; i < descripteurs.size(); i++)
    {
        if (nouveauDescripteur.getSource() == descripteurs[i].getSource())
        {
            throw std::invalid_argument("L'image existe déjà dans cette base de donnée.");
        }
    }
    descripteurs.push_back(nouveauDescripteur);
}

void Bibliotheque::enleveDescripteur(string &source)
{
    for (Descripteur d : descripteurs)
    {
        if (d.getSource() == source)
        {
            descripteurs.erase(remove_if(descripteurs.begin(), descripteurs.end(), [source](Descripteur d)
                                         { return d.getSource() == source; }));
        }
    }
}

// const vector<Utilisateur> &Bibliotheque::getUtilisateurs() const
// {
//     return utilisateurs;
// }

// void Bibliotheque::setUtilisateurs(const vector<Utilisateur> &utilisateurs)
// {
//     this->utilisateurs = utilisateurs;
// }

// les méthodes
// void Bibliotheque::creerBibliotheque()
// {
// }

// void Bibliotheque::modifierBibliotheque()
// {
// }

void Bibliotheque::supprimerBibliotheque()
{
    this->descripteurs.clear();
}

void Bibliotheque::chargerBibliotheque()
{
}

void Bibliotheque::sauvegarderBibliotheque()
{
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMin()
{
    auto descripteur = max_element(descripteurs.begin(), descripteurs.end(), comparerParCout);
    return descripteur->getCout();
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMax()
{
    auto descripteur = min_element(descripteurs.begin(), descripteurs.end(), comparerParCout);
    return descripteur->getCout();
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMoyen()
{
    if (descripteurs.size() == 0)
        return 0;

    double accumulator = 0;
    for (Descripteur d : descripteurs)
    {
        accumulator += d.getCout();
    }

    return accumulator / descripteurs.size();
}

vector<Descripteur> Bibliotheque::filter()
{
}

vector<Descripteur> Bibliotheque::trierDescripteurs()
{
    vector<Descripteur> descripteursTries = descripteurs;
    sort(descripteursTries.begin(), descripteursTries.end(), comparerParCout);
    return descripteursTries;
}

/**
 * @param cout
 */
void Bibliotheque::chercherImage(double cout)
{
}