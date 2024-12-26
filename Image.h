
#ifndef _IMAGE_H
#define _IMAGE_H

#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;

class Image
{
public:
    // Constructeur par défaut
    Image();

    // Constructeur avec paramètres
    Image(const Mat& data, const std::string& format, bool couleur, float tauxCompression);

    // Constructeur avec un paramètre Mat
    Image(const Mat& data);
    ~Image();

        // Getters and Setters
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

    void seuillage();

    void transformeeHough();

    
	// segmentation couleur ou noir et blanc
    Mat segmentationCouleurOuNG(const cv::Mat& imageOriginale,
        uchar seuilBasR, uchar seuilHautR,
        uchar seuilBasG, uchar seuilHautG,
        uchar seuilBasB, uchar seuilHautB);

    // affichage de l'image en mode teinte
    Mat afficherTeinte(const cv::Mat& image);
    // segmenter l'image avec un masque de teinte seuillé
    void segmenterParTeinte(const cv::Mat& image, const cv::Mat& hue, int seuilBas, int seuilHaut,int taillekernel);

private:
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