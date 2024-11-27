/**
 * Project Untitled
 */

#ifndef _IMAGE_H
#define _IMAGE_H

#include <opencv2/opencv.hpp>
using namespace cv;

class Image
{
public:
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