#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <string>
#include <vector>
#include <stdexcept>

class Utilisateur {
public:
    enum class Niveau { NIVEAU1, NIVEAU2, NIVEAU3 };

    // Constructeurs
    Utilisateur(); // Constructeur par défaut
    Utilisateur(const std::string& code); // Constructeur avec code

    // Getters
    std::string getCode() const;
    Niveau getNiveau() const;

    // Méthodes pour la création, modification, et suppression
    void creerUtilisateur();
    void modifierUtilisateur();
    void supprimerUtilisateur();

    // Méthode pour afficher les informations de l'utilisateur
    void afficherUtilisateur() const;

private:
    // Attributs
    std::string code;
    Niveau niveau;

    // Méthodes privées
    void setNiveauFromCode();
    bool validerCode(const std::string& code);
};

#endif 