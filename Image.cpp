
#include "Image.h"
#include <iostream>
/**
 * Image implementation
 */
// Constructeur par défaut
Image::Image() : idImage(0), data(), Format(""), Couleur(false), TauxCompression(0.0f), Largeur(0), Hauteur(0), acces(2), titreImage("") {
}
Image::Image(const int idImage, const Mat& data, const std::string& format, 
bool couleur, float tauxCompression) : idImage(idImage), data(data), 
Format(format), Couleur(couleur), TauxCompression(tauxCompression), Largeur(data.cols), Hauteur(data.rows), acces(2), titreImage("") {
}
// constructeur avec un paramètre si couleur retourne false = NGR, true = RGB
Image::Image(Mat& data) : data(data),  // Initialisation de 'data'
      Format(""),  // Initialisation de 'Format'
      Couleur(data.channels() > 1), // Initialisation de 'Couleur'
      TauxCompression(1.0f), // Valeur par défaut pour 'TauxCompression'
      Largeur(data.cols), // Initialisation de 'Largeur'
      Hauteur(data.rows), // Initialisation de 'Hauteur'
      acces(2), // Valeur par défaut pour 'acces'
      titreImage("") {}


   

// Destructeur
Image::~Image() {
    std::cout << "Destructeur de Image appelé" << std::endl;
    // Pas besoin de libérer 'data', cv::Mat gère cela automatiquement
}


// Implémentation des getters et setters
int Image::getIdImage() const 
{ return idImage; }
void Image::setIdImage(int id) 
{ idImage = id; }

const std::string& Image::getTitreImage() const 
{ return titreImage; }
void Image::setTitreImage(const std::string& titre) 
{ titreImage = titre; }

int Image::getLargeur() const 
{ return Largeur; }
void Image::setLargeur(int largeur) 
{ Largeur = largeur; }

int Image::getHauteur() const 
{ return Hauteur; }
void Image::setHauteur(int hauteur) 
{ Hauteur = hauteur; }

const Mat& Image::getData() const {
    return data;
}

const std::string& Image::getFormat() const {
    return Format;
}

bool Image::isCouleur() const {
    return Couleur;
}

float Image::getTauxCompression() const {
    return TauxCompression;
}

int Image::getAcces() const {
    return acces;
}

void Image::setAcces(int acces) {
    this->acces = acces;
}

// les méthodes
void Image::afficherImage () const
{
    if (!data.empty()) {
        imshow("Affichage Image", data);
        std::cout << "Affichage de l'image" << std::endl;
        waitKey(0);
    } else {
        std::cerr << "Erreur: aucune image à afficher" << std::endl;
    }
}

/**
 * @param Image
 */
void Image::histogramme()
{
}

/**
 * @param Filtre
 */
void Image::filtrageConvolution()
{
}

/**
 * @param Filtre
 */
void Image::detectionContours()
{
}

void Image::rehaussementContour()
{
}

void Image::segmentation()
{
}

void Image::seuillage()
{
}

void Image::transformeeHough()
{
}

Mat Image::segmentationContour() {
    cv::Mat grayImage, edges, closed;
    
    // Conversion en niveaux de gris si l'image est en couleur
    if (data.channels() > 1) {
        cv::cvtColor(data, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = data.clone();
    }
    
    // Détection de contours avec Canny
    cv::Canny(grayImage, edges, 100, 200);
    
    // Élément structurant pour la fermeture
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    
    // Opération de fermeture
    cv::morphologyEx(edges, closed, cv::MORPH_CLOSE, kernel);
    
    // Affichage de l'image résultante
    cv::imshow("Résultat de la segmentation", closed);
    std::cout << "segmentation terminee"<<std::endl;
    cv::waitKey(0);
    
    return closed;
}
