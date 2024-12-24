
#ifndef _IMAGE_H
#define _IMAGE_H

#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;

class Image
{
public:
    // Constructeur et Destructeur
    Image();
    Image(const Mat& data, const std::string& format, bool couleur, float tauxCompression);
    ~Image();

        // Getters and Setters
    const Mat& getData() const;
    const std::string& getFormat() const;
    bool isCouleur() const;
    float getTauxCompression() const;
    int getAcces() const;
    void setAcces(int acces);

    // les méthodes
    void afficherImage();

    /**
     * @param Image
     */
    void histogramme();

    /**
     * @param Filtre
     */
    void filtrageConvolution();

    /**
     * @param Filtre
     */
    void detectionContours();

    void rehaussementContour();

    void segmentation();

    void seuillage();

    void transforméeHough();

private:
    const Mat data;
    const String Format;
    const bool Couleur;
    const float TauxCompression;
    int acces;
};

#endif //_IMAGE_H