
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
        std::cerr << "Erreur: aucune image a afficher" << std::endl;
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


// segmentation couleur ou noir et blanc
cv::Mat Image::segmentationCouleurOuNG(const cv::Mat& imageOriginale,
    uchar seuilBasR, uchar seuilHautR,
    uchar seuilBasG, uchar seuilHautG,
    uchar seuilBasB, uchar seuilHautB) {

    // Créer une image binaire pour le masque
    cv::Mat masque = cv::Mat::zeros(imageOriginale.size(), CV_8U);

    // Vérifier si l'image est en couleur (RGB) ou en niveaux de gris (NG)
    if (imageOriginale.channels() == 3) { // Image RGB
        for (int y = 0; y < imageOriginale.rows; y++) {
            for (int x = 0; x < imageOriginale.cols; x++) {
                // Obtenir les valeurs des canaux R, G et B pour l'image RGB
                cv::Vec3b pixel = imageOriginale.at<cv::Vec3b>(y, x); // Le pixel est un vecteur avec 3 valeurs : R, G, B
                uchar valeurR = pixel[2]; // Canal Rouge (index 2)
                uchar valeurG = pixel[1]; // Canal Vert (index 1)
                uchar valeurB = pixel[0]; // Canal Bleu (index 0)

                // Vérifier si la couleur du pixel est dans les intervalles définis
                if (valeurR >= seuilBasR && valeurR <= seuilHautR &&
                    valeurG >= seuilBasG && valeurG <= seuilHautG &&
                    valeurB >= seuilBasB && valeurB <= seuilHautB) {
                    masque.at<uchar>(y, x) = 255; // Pixel dans l'intervalle
                }
            }
        }
    }
    else if (imageOriginale.channels() == 1) { // Image en niveaux de gris
        for (int y = 0; y < imageOriginale.rows; y++) {
            for (int x = 0; x < imageOriginale.cols; x++) {
                uchar valeurNG = imageOriginale.at<uchar>(y, x); // Valeur du pixel en NG

                // Utiliser uniquement le seuil rouge pour le seuillage
                if (valeurNG >= seuilBasR && valeurNG <= seuilHautR) {
                    masque.at<uchar>(y, x) = 255; // Pixel au-dessus du seuil rouge
                }
            }
        }
    }

    // Appliquer le masque à l'image originale pour obtenir l'image segmentée
    cv::Mat imageSegmentee;
    imageOriginale.copyTo(imageSegmentee, masque);

    // Afficher le résultat
    cv::imshow("image segmentee", imageSegmentee);
    cv::waitKey(0); // Attendre une touche pour fermer la fenêtre

    return imageSegmentee;
}

