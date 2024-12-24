
#include "Image.h"
#include <iostream>
/**
 * Image implementation
 */
// Constructeur par défaut
Image::Image() : data(), Format(""), Couleur(false), TauxCompression(0.0f), acces(0) {
}
// Constructeur paramétré
Image::Image(const Mat& data, const std::string& format, bool couleur, float tauxCompression)
    : data(data), Format(format), Couleur(couleur), TauxCompression(tauxCompression), acces(0) {
}
// Destructeur
Image::~Image() {
}

// Getters and Setters
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
void Image::afficherImage()
{
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

void Image::transforméeHough()
{
}