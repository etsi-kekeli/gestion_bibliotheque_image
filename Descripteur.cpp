
#include "Descripteur.h"

/**
 * Descripteur implementation
 */
// Default constructor ipar default
Descripteur::Descripteur() : IdDescripteur(0), source(""), cout(0.0) {
    
}
// constructor paramétré
Descripteur::Descripteur(int id, const std::string& titre, const std::string& source, double cout, const std::string& auteur)
    : IdDescripteur(id), Titre(titre), source(source), cout(cout), Auteur(auteur) {
    
}
// Destructeur
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

void Descripteur::CréerDescripteur() {

}

void Descripteur::ModifierDescripteur() {

}

void Descripteur::getters() {

}

void Descripteur::setters() {

}

void Descripteur::modifierDescripteur() {

}

void Descripteur::supprimerDescripteur() {

}

void Descripteur::creerDescripteur() {

}