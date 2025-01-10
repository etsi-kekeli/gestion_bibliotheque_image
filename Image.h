#ifndef _IMAGE_H
#define _IMAGE_H

#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;

// déclaration des enumérations des types des filtres implementés
enum class FilterType {
    MEAN,
    SOBEL_X,
    SOBEL_Y,
    LAPLACIAN,
    GAUSSIAN
};

class Image
{
public:
    // Constructeur par défaut
    Image();

    // Constructeur avec paramètres
    Image(const Mat& data, const std::string& format, bool couleur, float tauxCompression);

    // Constructeur avec un paramètre Mat
    Image(const Mat& data);

    // Constructeur avec un parametre source
    Image(const std::string& source);


    // Destructeur
    ~Image();

    // Getters and Setters
    const std::string& getTitreImage() const;
    void setTitreImage(const std::string& titre);
    const std::string& getSource() const;
    const Mat& getData() const;
    const std::string& getFormat() const;
    bool isCouleur() const;
    float getTauxCompression() const;

    Image& operator=(const Image& other);

    // les méthodes
    void afficherImage() const;

    void calculateAndDisplayHistogram(const Mat& image);
     
	// Méthode pour appliquer une convolution 2D (Manel)
    Mat convolution2D(const Mat& src, FilterType filterType);


    // Méthode pour détecter les contours (True pour Gradient False pour Laplacien (Arame)
    void detectionContours(bool useGradient);

    // Méthode pour calculer Magnitude afin de detecter le contour par le Gradient (Arame)
    Mat calculateMagnitude(const Mat& gradX, const Mat& gradY);

    void rehaussementContour();

    void seuillage();

	// Méthode pour appliquer la transformee de Hough (Samuel)
    Mat transformeeHough(int nRho = 360, int nTheta = 300);
    Mat getTableDAccumulationNormalisee(int nRho = 360, int nTheta = 300);
    Mat dessineLigneHough(int nRho, int nTheta, int tailleVoisinage, int seuil, Scalar couleur = Scalar(255, 255, 0), int epaisseur = 2);

    // segmentation couleur ou noir et blanc (Amine)
    Mat segmentationCouleurOuNG(const cv::Mat& imageOriginale,
        uchar seuilBasR, uchar seuilHautR,
        uchar seuilBasG, uchar seuilHautG,
        uchar seuilBasB, uchar seuilHautB);

    // affichage de l'image en mode teinte (Amine)
    Mat afficherTeinte(const cv::Mat& image);
    // segmenter l'image avec un masque de teinte seuillé (Amine)
    void segmenterParTeinte(const cv::Mat& image, const cv::Mat& hue, int seuilBas, int seuilHaut, int taillekernel);

private:
    std::string titreImage;
    const Mat data;
    String Format;
    const bool Couleur;
    const float TauxCompression;
    std::string source;

};
void _dessineLigneHough(Mat& image, float rho, float theta, Scalar couleur, int epaisseur);

#endif //_IMAGE_H