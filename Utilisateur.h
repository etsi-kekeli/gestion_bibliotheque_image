/**
 * Project Untitled
 */


#ifndef _UTILISATEUR_H
#define _UTILISATEUR_H

class Utilisateur {
public: 
    
void getNiveauAcces();
    
void créerUtilisateur();
    
void modifierUtilisateur();
    
void supprimerUtilisateur();
    
void authentifier();
private: 
    String CodeUtilisateur;
    String NomUtilisateur;
};

#endif //_UTILISATEUR_H