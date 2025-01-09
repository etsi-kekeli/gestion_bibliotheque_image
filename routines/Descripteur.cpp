#include "Descripteur.h"
#include <iostream>
#include <limits>
#include <string>

/**
 * Descripteur implementation
 */
// Initialisation du set statique
std::set<std::string> Descripteur::sourcesUtilisees;

// constructeur par default
Descripteur::Descripteur() : IdDescripteur(0), source(""), cout(0.0)
{
}
// Constructeur paramétré
Descripteur::Descripteur(int id, const std::string &titre, std::string &source, double cout, const std::string &auteur)
    : IdDescripteur(id), Titre(titre), source(source), cout(cout), Auteur(auteur)
{

    // Vérifier l'unicité de la source
    if (sourcesUtilisees.find(source) != sourcesUtilisees.end())
    {
        throw std::invalid_argument("La source doit être unique.");
    }

    sourcesUtilisees.insert(source);
} // Destructeur
Descripteur::~Descripteur()
{
}
// Getters and Setters
int Descripteur::getIdDescripteur() const
{
    return IdDescripteur;
}

void Descripteur::setIdDescripteur(int id)
{
    IdDescripteur = id;
}

const std::string &Descripteur::getTitre() const
{
    return Titre;
}

void Descripteur::setTitre(const std::string &titre)
{
    Titre = titre;
}

const std::string &Descripteur::getSource() const
{
    return source;
}

double Descripteur::getCout() const
{
    return cout;
}

void Descripteur::setCout(double cout)
{
    this->cout = cout;
}

const std::string &Descripteur::getAuteur() const
{
    return Auteur;
}

void Descripteur::setAuteur(const std::string &auteur)
{
    Auteur = auteur;
}

Acces Descripteur::getAcces() const
{
    return acces;
}
void Descripteur::setAcces(Acces acces)
{
    this->acces = acces;
}

// les méthodes

// pour la création, modification et supression du descripteur, le descripteur doit etre crée dynamiquement
// cela permet de libérer de la mémoire au cours de l'execution de programme
// Créer un nouveau descripteur (ici source est l'identifiant de l'image il est unique)
void Descripteur::creerDescripteur(const Image &image)
{
    std::string titre, auteur;

    std::cout << "Creation d'un nouveau descripteur\n";

    std::cout << "Entrez le titre de l'image : ";
    std::getline(std::cin, titre);
    Titre = titre;

    // Demander à l'utilisateur de saisir la source
    do
    {
        std::cout << "Entrez la source (identifiant unique) de l'image : ";
        std::getline(std::cin, source);

        // Vérifier l'unicité de la source
        if (source.empty())
        {
            std::cout << "La source ne peut pas etre vide. Veuillez reessayer." << std::endl;
        }
        else if (sourcesUtilisees.find(source) != sourcesUtilisees.end())
        {
            std::cout << "Cette source est deja presente. Veuillez entrer une nouvelle source." << std::endl;
        }
    } while (source.empty() || sourcesUtilisees.find(source) != sourcesUtilisees.end());

    std::cout << "Entrez le cout de l'image : ";
    std::cin >> cout;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Entrez l'auteur de l'image : ";
    std::getline(std::cin, auteur);
    this->Auteur = auteur;

    sourcesUtilisees.insert(source); // Ajouter la nouvelle source au set

    std::cout << "Descripteur cree avec succes.\n";
}

// Modifier un descripteur ( titre, cout, auteur) les autres ne sont pas modifiables
void Descripteur::modifierDescripteur()
{
    std::string nouveauTitre, nouvelAuteur;
    double nouveauCout;

    std::cout << "Modification du descripteur\n";

    // Modifier le titre
    std::cout << "Entrez le nouveau titre (laisser vide pour ne pas changer) : ";
    std::getline(std::cin, nouveauTitre);
    if (!nouveauTitre.empty())
    {
        Titre = nouveauTitre; // Mettre à jour le titre seulement s'il n'est pas vide
    }

    // Modifier le coût
    std::cout << "Entrez le nouveau cout (laisser vide pour ne pas changer) : ";
    std::string inputCout;
    std::getline(std::cin, inputCout);
    if (!inputCout.empty())
    {
        try
        {
            nouveauCout = std::stod(inputCout); // Convertir en double
            cout = nouveauCout;                 // Mettre à jour le coût
        }
        catch (const std::invalid_argument &)
        {
            std::cerr << "Cout invalide. Aucune modification effectuee.\n";
        }
    }

    // Modifier l'auteur
    std::cout << "Entrez le nouvel auteur (laisser vide pour ne pas changer) : ";
    std::getline(std::cin, nouvelAuteur);
    if (!nouvelAuteur.empty())
    {
        Auteur = nouvelAuteur; // Mettre à jour l'auteur seulement s'il n'est pas vide
    }

    std::cout << "Descripteur modifie avec succes.\n";
}
// Supprimer un descripteur
void Descripteur::supprimerDescripteur()
{
    std::cout << "Suppression du descripteur\n";
    std::cout << "Titre: " << Titre << "\n";
    std::cout << "Cout: " << cout << "\n";
    std::cout << "Auteur: " << Auteur << "\n";
    std::cout << "Source: " << source << "\n";

    // Supprimer la source de l'ensemble pour maintenir l'unicité
    sourcesUtilisees.erase(source);

    // Réinitialiser les attributs
    Titre = "";
    source = "";
    cout = 0.0;
    Auteur = "";

    // Suppression de l'instance
    delete this; // Attention : cela ne doit être fait que si l'objet a été créé avec 'new'

    std::cout << "Descripteur supprime avec succes.\n";
}

// int IdDescripteur;
// std::string Titre;
// std::string source;
// double cout;
// std::string Auteur;
// Acces acces;

void Descripteur::serialiser(std::ofstream &fichier) const
{
    fichier.write(reinterpret_cast<const char *>(&IdDescripteur), sizeof(IdDescripteur));

    size_t longueurTitre = Titre.length();
    fichier.write(reinterpret_cast<const char *>(&longueurTitre), sizeof(longueurTitre));
    fichier.write(Titre.c_str(), longueurTitre);

    size_t longueurSource = source.size();
    fichier.write(reinterpret_cast<const char *>(&longueurSource), sizeof(longueurSource));
    fichier.write(source.c_str(), longueurSource);

    fichier.write(reinterpret_cast<const char *>(&cout), sizeof(cout));

    size_t longueurAuteur = Auteur.size();
    fichier.write(reinterpret_cast<const char *>(&longueurAuteur), sizeof(longueurAuteur));
    fichier.write(Auteur.c_str(), longueurAuteur);

    fichier.write(reinterpret_cast<const char *>(&acces), sizeof(acces));
}

void Descripteur::deserialiser(std::ifstream &fichier)
{
    fichier.read(reinterpret_cast<char *>(&IdDescripteur), sizeof(IdDescripteur));

    size_t longueurTitre;
    fichier.read(reinterpret_cast<char *>(&longueurTitre), sizeof(longueurTitre));
    Titre.resize(longueurTitre);
    fichier.read(&Titre[0], longueurTitre);

    size_t longueurSource;
    fichier.read(reinterpret_cast<char *>(&longueurSource), sizeof(longueurSource));
    source.resize(longueurSource);
    fichier.read(&source[0], longueurSource);

    fichier.read(reinterpret_cast<char *>(&cout), sizeof(cout));

    size_t longueurAuteur;
    fichier.read(reinterpret_cast<char *>(&longueurAuteur), sizeof(longueurAuteur));
    source.resize(longueurAuteur);
    fichier.read(&Auteur[0], longueurAuteur);

    fichier.read(reinterpret_cast<char *>(&acces), sizeof(acces));
}

bool comparerParCout(Descripteur &d1, Descripteur &d2)
{
    return d1.getCout() < d2.getCout();
}