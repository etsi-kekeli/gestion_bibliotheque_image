/**
 * Project Untitled
 */

#include "Bibliotheque.h"

/**
 * Bibliothèque implementation
 */
// Constructeur par défaut
Bibliotheque::Bibliotheque() {
    
}
// Constructeur paramétré
Bibliotheque::Bibliotheque(const std::string& nom, const std::vector<Descripteur>& descripteurs, const std::vector<Utilisateur>& utilisateurs)
    : nom(nom), descripteurs(descripteurs), utilisateurs(utilisateurs) {
}
// Destructeur
Bibliotheque::~Bibliotheque() {
    
}

// Getters and Setters
const std::string& Bibliotheque::getNom() const {
    return nom;
}

void Bibliotheque::setNom(const std::string& nom) {
    this->nom = nom;
}

const std::vector<Descripteur>& Bibliotheque::getDescripteurs() const {
    return descripteurs;
}

void Bibliotheque::setImages(const std::vector<Descripteur>& descripteurs) {
    this->descripteurs = descripteurs;
}

const std::vector<Utilisateur>& Bibliotheque::getUtilisateurs() const {
    return utilisateurs;
}

void Bibliotheque::setUtilisateurs(const std::vector<Utilisateur>& utilisateurs) {
    this->utilisateurs = utilisateurs;
}


void Bibliotheque::creerBibliotheque()
{
}

void Bibliotheque::modifierBibliotheque()
{
}

void Bibliotheque::supprimerBibliotheque()
{
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
    return 0.0;
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMax()
{
    return 0.0;
}

/**
 * @return double
 */
double Bibliotheque::calculerCoutMoyen()
{
}

void Bibliotheque::filter()
{
}

void Bibliotheque::ajouterDescripteur()
{
}

void Bibliotheque::enleveDescripteur()
{
}

void Bibliotheque::trierImage()
{
}

/**
 * @param cout
 */
void Bibliotheque::chercherImage(double cout)
{
}