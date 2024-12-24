
#ifndef _UTILISATEUR_H
#define _UTILISATEUR_H
#include <string>

class Utilisateur {
public: 
    // Construscteur et Destructeur
    Utilisateur();
    ~Utilisateur();

     // Getters and Setters
    const std::string& getCodeUtilisateur() const;
    void setCodeUtilisateur(const std::string& CodeUtilisateur);

    const std::string& getNomUtilisateur() const;
    void setNomUtilisateur(const std::string& NomUtilisateur);


    // les méthodes
    void getNiveauAcces();
        
    void créerUtilisateur();
        
    void modifierUtilisateur();
        
    void supprimerUtilisateur();
        
    void authentifier();
private: 
    std::string CodeUtilisateur;
    std::string NomUtilisateur;
};

#endif //_UTILISATEUR_H