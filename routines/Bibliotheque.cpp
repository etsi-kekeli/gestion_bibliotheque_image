#include "Bibliotheque.h"
#include <algorithm>
using namespace std;

const int Bibliotheque::signature = 2025;


/**
 * Bibliothèque implementation
 */
// Constructeur par défaut
Bibliotheque::Bibliotheque()
{
    nom = "bibliothèque";
    descripteurs = new vector<Descripteur*>();
}

// Constructeur paramétré
Bibliotheque::Bibliotheque(const string &nom, vector<Descripteur*> *descripteurs)
    : nom(nom), descripteurs(descripteurs)
{
}
// Destructeur
Bibliotheque::~Bibliotheque()
{
    for (Descripteur* d: *descripteurs){
        if (d != nullptr) delete d;
    }

    if (descripteurs!= nullptr) delete descripteurs;
}

// Getters and Setters
const string &Bibliotheque::getNom() const
{
    return nom;
}

void Bibliotheque::setNom(string nom)
{
    this->nom = nom;
}

vector<Descripteur*>* Bibliotheque::getDescripteurs() const
{
    return descripteurs;
}

void Bibliotheque::setImages(vector<Descripteur*> *descripteurs)
{
    this->descripteurs = descripteurs;
}

void Bibliotheque::ajouterDescripteur(Descripteur *nouveauDescripteur)
{
    for (size_t i = 0; i < descripteurs->size(); i++)
    {
        if (nouveauDescripteur->getSource() == (*descripteurs)[i]->getSource())
        {
            throw std::invalid_argument("L'image existe déjà dans cette base de donnée.");
        }
    }
    descripteurs->push_back(nouveauDescripteur);
}

void Bibliotheque::enleveDescripteur(const string &source)
{
    for (Descripteur* d : *descripteurs)
    {
        if (d->getSource() == source)
        {
            descripteurs->erase(remove_if(descripteurs->begin(), descripteurs->end(), [source](Descripteur* d)
                                         { return d->getSource() == source; }));
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
    this->descripteurs->clear();
}

bool Bibliotheque::chargerBibliotheque(string nomDuFichier)
{
    ifstream fichierSauvegarde(nomDuFichier, ios::binary);
    if (!fichierSauvegarde)
        return false;

    // int test;
    // fichierSauvegarde.read(reinterpret_cast<char *>(&test), sizeof(test));

    // if (test != signature) return false;

    size_t longueurNom;
    fichierSauvegarde.read(reinterpret_cast<char *>(&longueurNom), sizeof(longueurNom));
    nom.resize(longueurNom);
    fichierSauvegarde.read(&nom[0], longueurNom);

    size_t nombreDescripteur;
    fichierSauvegarde.read(reinterpret_cast<char *>(&nombreDescripteur), sizeof(nombreDescripteur));

    descripteurs = new vector<Descripteur*>();

    for (size_t i = 0; i < nombreDescripteur; i++)
    {
        Descripteur *d = new Descripteur();
        d->deserialiser(fichierSauvegarde);
        descripteurs->push_back(d);
    }

    fichierSauvegarde.close();
    return true;
}

// string nom;
// vector<Descripteur> descripteurs;
bool Bibliotheque::sauvegarderBibliotheque(string nomDuFichier) const
{
    ofstream fichierSauvegarde(nomDuFichier, ios::binary);

    if (!fichierSauvegarde)
        return false;

    // fichierSauvegarde.write(reinterpret_cast<const char *>(&signature), sizeof(signature));

    size_t longueurNom = nom.size();
    fichierSauvegarde.write(reinterpret_cast<const char *>(&longueurNom), sizeof(longueurNom));
    fichierSauvegarde.write(nom.c_str(), longueurNom);

    size_t nombreDescripteur = descripteurs->size();
    fichierSauvegarde.write(reinterpret_cast<const char *>(&nombreDescripteur), sizeof(nombreDescripteur));

    for (Descripteur *d : *descripteurs)
    {
        d->serialiser(fichierSauvegarde);
    }

    fichierSauvegarde.close();
    return true;
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMin()
{
    if (descripteurs->empty()) return 0.0;
    auto descripteur = min_element(descripteurs->begin(), descripteurs->end(), comparerParCout);
    return (*descripteur)->getCout();
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMax()
{
    if (descripteurs->empty()) return 0.0;
    auto descripteur = max_element(descripteurs->begin(), descripteurs->end(), comparerParCout);
    return (*descripteur)->getCout();
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMoyen()
{
    if (descripteurs->size() == 0)
        return 0;

    double accumulator = 0;
    for (Descripteur *d : *descripteurs)
    {
        accumulator += d->getCout();
    }

    return accumulator / descripteurs->size();
}

int Bibliotheque::nombreDImagesLibres()
{
    int compteur = 0;
    for (Descripteur *d : *descripteurs)
    {
        if (d->getAcces() == Acces::OUVERT)
            compteur++;
    }

    return compteur;
}

vector<Descripteur*>* Bibliotheque::filter(double coutMin, double coutMax)
{
    vector<Descripteur*>* resultat = new vector<Descripteur*>();

    if (coutMax >= coutMin)
        for (Descripteur *d : *descripteurs)
        {
            if (d->getCout() >= coutMin && d->getCout() <= coutMax)
            {
                resultat->push_back(d);
            }
        }

    return resultat;
}

vector<Descripteur*>* Bibliotheque::trierDescripteurs()
{
    vector<Descripteur*>* descripteursTries = descripteurs;

    sort(descripteursTries->begin(), descripteursTries->end(), comparerParCout);
    return descripteursTries;
}

/**
 * @param cout
 */
double Bibliotheque::chercherCoutImage(int ID)
{
    for (Descripteur *d : *descripteurs)
    {
        if (d->getIdDescripteur() == ID)
            return d->getCout();
    }

    return -1;
}
