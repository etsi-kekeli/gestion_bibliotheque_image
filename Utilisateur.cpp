
#include "Utilisateur.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <limits>

// Constructeur par défaut
Utilisateur::Utilisateur() : code(""), niveau(Niveau::NIVEAU1) {}

// Constructeur paramétré
Utilisateur::Utilisateur(const std::string& code) : code(code) {
    setNiveauFromCode();
}

// Getter pour le code
std::string Utilisateur::getCode() const {
    return code;
}

// Getter pour le niveau
Utilisateur::Niveau Utilisateur::getNiveau() const {
    return niveau;
}

// Déterminer le niveau d'accès en fonction du code
void Utilisateur::setNiveauFromCode() {
    if (code.rfind("AB", 0) == 0) {
        niveau = Niveau::NIVEAU1;
    }
    else if (code.rfind("CD", 0) == 0) {
        niveau = Niveau::NIVEAU2;
    }
    else if (code.rfind("EF", 0) == 0) {
        niveau = Niveau::NIVEAU3;
    }
    else {
        throw std::invalid_argument("Code invalide : doit commencer par AB, CD ou EF.");
    }
}

// Validation du code utilisateur
bool Utilisateur::validerCode(const std::string& code) {
    std::regex pattern("[A-Z]{2}-[0-9]{2}-[A-Z]{2}");
    return std::regex_match(code, pattern);
}

// Méthodes pour la création, modification, et suppression

// Création d'un utilisateur
void Utilisateur::creerUtilisateur() {
    std::string nouveauCode;

    std::cout << "Creation d'un utilisateur\n";

    do {
        std::cout << "Entrez le code utilisateur (format : XX-YY-ZZ avec AB, CD ou EF en prefixe) : ";
        std::getline(std::cin, nouveauCode);

        if (nouveauCode.empty()) {
            std::cout << "Le code ne peut pas etre vide. Veuillez reessayer.\n";
        }
        else if (!validerCode(nouveauCode)) {
            std::cout << "Le code est invalide. Veuillez respecter le format.\n";
        }
    } while (nouveauCode.empty() || !validerCode(nouveauCode));

    code = nouveauCode;
    setNiveauFromCode();

    std::cout << "Utilisateur cree avec succes : " << code << "\n";
    afficherUtilisateur(); // Affichage après création
}

// Modifier l'utilisateur
void Utilisateur::modifierUtilisateur() {
    std::cout << "Modification de l'utilisateur\n";

    std::string nouveauCode;
    std::cout << "Entrez le nouveau code utilisateur (laisser vide pour ne pas changer) : ";
    std::getline(std::cin, nouveauCode);

    if (!nouveauCode.empty()) {
        if (!validerCode(nouveauCode)) {
            std::cout << "Code invalide. Aucune modification effectuee.\n";
        }
        else {
            code = nouveauCode;
            setNiveauFromCode();
            std::cout << "Code modifie avec succes : " << code << "\n";
        }
    }
    else {
        std::cout << "Aucune modification effectuee.\n";
    }

    afficherUtilisateur(); // Affichage après modification
}

// Supprimer un utilisateur
void Utilisateur::supprimerUtilisateur() {
    std::cout << "Suppression de l'utilisateur\n";
    std::cout << "Utilisateur avec le code " << code << " va etre supprime.\n";

    // Réinitialisation des attributs
    code = "";
    niveau = Niveau::NIVEAU1;

    std::cout << "Utilisateur supprime avec succes.\n";
}

// Afficher les informations de l'utilisateur
void Utilisateur::afficherUtilisateur() const {
    std::cout << "\nInformations utilisateur:\n";
    std::cout << "Code : " << code << "\n";
    std::cout << "Niveau d'accès : ";
    switch (niveau) {
    case Niveau::NIVEAU1:
        std::cout << "NIVEAU1\n";
        break;
    case Niveau::NIVEAU2:
        std::cout << "NIVEAU2\n";
        break;
    case Niveau::NIVEAU3:
        std::cout << "NIVEAU3\n";
        break;
    }
}


/*// Constructeur
Utilisateur::Utilisateur(std::string code) {
    validerCode(code);
    this->code = code;
    this->niveau = determinerNiveau(code);
}

// Valider que le code respecte le format attendu
void Utilisateur::validerCode(const std::string& code) {
    std::regex pattern("[A-Z]{2}-[0-9]{2}-[A-Z]{2}");
    if (!std::regex_match(code, pattern)) {
        throw std::invalid_argument("Code d'accès invalide : format incorrect. Attendu : XX-99-XX.");
    }

    std::string prefix = code.substr(0, 2);
    if (prefix != "AB" && prefix != "CD" && prefix != "EF") {
        throw std::invalid_argument("Code d'accès invalide : préfixe incorrect.");
    }
}

// Déterminer le niveau d'accès en fonction du préfixe
Utilisateur::Niveau Utilisateur::determinerNiveau(const std::string& code) const {
    std::string prefix = code.substr(0, 2);
    if (prefix == "AB") return Niveau::NIVEAU1;
    if (prefix == "CD") return Niveau::NIVEAU2;
    if (prefix == "EF") return Niveau::NIVEAU3;

    throw std::invalid_argument("Code d'accès invalide : préfixe non reconnu.");
}

// Getters
std::string Utilisateur::getCode() const {
    return code;
}

Utilisateur::Niveau Utilisateur::getNiveau() const {
    return niveau;
}

bool Utilisateur::aAccesTotal() const {
    return niveau == Niveau::NIVEAU1;
}

bool Utilisateur::aAccesConsultation() const {
    return niveau == Niveau::NIVEAU1 || niveau == Niveau::NIVEAU2;
}

// Définir les images autorisées pour un utilisateur de niveau 1 uniquement
void Utilisateur::definirImagesAutorisees(const std::vector<std::string>& images) {
    if (niveau == Niveau::NIVEAU1) {
        imagesAutorisees = images;
    }
    else {
        std::cerr << "Seul un utilisateur de niveau 1 peut définir les images autorisées.\n";
    }
}

// Obtenir les images autorisées
std::vector<std::string> Utilisateur::getImagesAutorisees() const {
    return imagesAutorisees;
}



/*Utilisateur::Utilisateur(std::string code, Niveau niveau) : code(code), niveau(niveau) {
    // Validation du format du code
    std::regex pattern("[A-Z]{2}-[0-9]{2}-[A-Z]{2}");
    if (!std::regex_match(code, pattern)) {
        throw std::invalid_argument("Code d'accès invalide : format incorrect.");
    }

    // Validation du préfixe en fonction du niveau
    if ((niveau == Niveau::NIVEAU1 && code.substr(0, 2) != "AB") ||
        (niveau == Niveau::NIVEAU2 && code.substr(0, 2) != "CD") ||
        (niveau == Niveau::NIVEAU3 && code.substr(0, 2) != "EF")) {
        throw std::invalid_argument("Code d'accès invalide : préfixe incorrect pour le niveau.");
    }

}

std::string Utilisateur::getCode() const {
    return code;
}

Utilisateur::Niveau Utilisateur::getNiveau() const {
    return niveau;
}

bool Utilisateur::aAccesTotal() const {
    return niveau == Niveau::NIVEAU1;
}

bool Utilisateur::aAccesConsultation() const {
    return niveau == Niveau::NIVEAU1 || niveau == Niveau::NIVEAU2;
}

void Utilisateur::definirImagesAutorisees(const std::vector<std::string>& images) {
    if (niveau == Niveau::NIVEAU1) {
        imagesAutorisees = images;
    }
    else {
        std::cerr << "Seul un utilisateur de niveau 1 peut définir les images autorisées.\n";
    }
}

std::vector<std::string> Utilisateur::getImagesAutorisees() const {
    return imagesAutorisees;
}
*/