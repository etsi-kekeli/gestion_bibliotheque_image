#include "Descripteur.h"
#include <iostream>
#include <limits>
#include <string>

/**
 * Descripteur implementation
 */
 // Initialisation du set statique
std::set<std::string> Descripteur::sourcesUtilisees;

// constructzue par default
Descripteur::Descripteur() : IdDescripteur(0), source(""), cout(0.0) {
    
}
// Constructeur paramétré
Descripteur::Descripteur(int id, const std::string& titre, std::string& source, double cout, const std::string& auteur)
    : IdDescripteur(id), Titre(titre), source(source), cout(cout), Auteur(auteur) {

    // Vérifier l'unicité de la source
    if (sourcesUtilisees.find(source) != sourcesUtilisees.end()) {
        throw std::invalid_argument("La source doit être unique.");
    }

    sourcesUtilisees.insert(source);
}// Destructeur
Descripteur::~Descripteur() {
    
}
// Getters and Setters
int Descripteur::getIdDescripteur() const {
    return IdDescripteur;
}

void Descripteur::setIdDescripteur(int id) {
    IdDescripteur = id;
}

const std::string& Descripteur::getTitre() const {
    return Titre;
}

void Descripteur::setTitre(const std::string& titre) {
    Titre = titre;
}

const std::string& Descripteur::getSource() const {
    return source;
}

double Descripteur::getCout() const {
    return cout;
}

void Descripteur::setCout(double cout) {
    this->cout = cout;
}

const std::string& Descripteur::getAuteur() const {
    return Auteur;
}

void Descripteur::setAuteur(const std::string& auteur) {
    Auteur = auteur;
}

// les méthodes 
void Descripteur::TrierImage() {

}

/**
 * @param cout
 */
void Descripteur::ChercherImage(double cout) {

}

void Descripteur::FilterImage() {

}

void Descripteur::modifierDescripteur() {

}

void Descripteur::supprimerDescripteur() {

}
// Créer un nouveau descripteur (ici source est l'identifiant de l'image il est unique)
void Descripteur::creerDescripteur(const Image& image) {
    std::string titre, auteur;

    std::cout << "Creation d'un nouveau descripteur\n";

    std::cout << "Entrez le titre de l'image : ";
    std::getline(std::cin, titre);
    Titre = titre;

    // Demander à l'utilisateur de saisir la source
    std::cout << "Entrez la source (identifiant unique) de l'image : ";
    std::getline(std::cin, source); 

    // Vérifier l'unicité de la source
    if (sourcesUtilisees.find(source) != sourcesUtilisees.end()) {
        throw std::invalid_argument("L'image est deja presente.");
    }

    std::cout << "Entrez le cout de l'image : ";
    std::cin >> cout;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Entrez l'auteur de l'image : ";
    std::getline(std::cin, auteur);
    this->Auteur = auteur;

    sourcesUtilisees.insert(source); // Ajouter la nouvelle source au set

    std::cout << "Descripteur cree avec succes.\n";
}
