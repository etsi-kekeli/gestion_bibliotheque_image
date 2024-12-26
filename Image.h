
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
    Image(const int idImage, const Mat& data, const std::string& format, 
     bool couleur, float tauxCompression); 
    Image(Mat& data);
    ~Image();

        // Getters and Setters
     int getIdImage() const;
    void setIdImage(int id);
    const std::string& getTitreImage() const;
    void setTitreImage(const std::string& titre);
    int getLargeur() const;
    void setLargeur(int largeur);
    int getHauteur() const;
    void setHauteur(int hauteur);
    const Mat& getData() const;
    const std::string& getFormat() const;
    bool isCouleur() const;
    float getTauxCompression() const;
    int getAcces() const;
    void setAcces(int acces);

    // les méthodes
    void afficherImage () const  ;

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

    void transformeeHough();

    Mat segmentationContour();
	// segmentation couleur ou noir et blanc
    Mat segmentationCouleurOuNG(const cv::Mat& imageOriginale,
        uchar seuilBasR, uchar seuilHautR,
        uchar seuilBasG, uchar seuilHautG,
        uchar seuilBasB, uchar seuilHautB);

private:
    int idImage;
    std::string titreImage;
    int Largeur;
    int Hauteur;
    const Mat data;
    const String Format;
    const bool Couleur;
    const float TauxCompression;
    int acces;
};

#endif //_IMAGE_H