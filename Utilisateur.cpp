#include "Utilisateur.h"

/**
 * Utilisateur implementation
 */
// Constructeur et Destructeur
Utilisateur::Utilisateur() {
    
}

Utilisateur::~Utilisateur() {

}

/// Getters and Setters
const std::string& Utilisateur::getCodeUtilisateur() const {
    return CodeUtilisateur;
}

void Utilisateur::setCodeUtilisateur(const std::string& CodeUtilisateur) {
    this->CodeUtilisateur = CodeUtilisateur;
}

const std::string& Utilisateur::getNomUtilisateur() const {
    return NomUtilisateur;
}

void Utilisateur::setNomUtilisateur(const std::string& NomUtilisateur) {
    this->NomUtilisateur = NomUtilisateur;
}

// Les méthodes
void Utilisateur::getNiveauAcces() {

}

void Utilisateur::créerUtilisateur() {

}

void Utilisateur::modifierUtilisateur() {

}

void Utilisateur::supprimerUtilisateur() {

}

void Utilisateur::authentifier() {

}