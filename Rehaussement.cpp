#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Chemin relatif vers l'image Brain2.jpeg dans le dossier BDD_Image
    string cheminImage = "routines/BDD_Image/Brain2.jpeg";

    // Charger l'image en niveaux de gris
    Mat imageGris = imread(cheminImage, IMREAD_GRAYSCALE);
    if (imageGris.empty()) {
        cerr << "Erreur : Impossible de charger l'image depuis " << cheminImage << endl;
        return -1;
    }

    // Charger l'image en couleur (pour le rehaussement)
    Mat imageCouleur = imread(cheminImage, IMREAD_COLOR);
    if (imageCouleur.empty()) {
        cerr << "Erreur : Impossible de charger l'image couleur depuis " << cheminImage << endl;
        return -1;
    }

    // Appliquer les gradients Sobel pour détecter les contours
    Mat gradX, gradY, magnitude;
    Sobel(imageGris, gradX, CV_64F, 1, 0, 3);  // Gradient horizontal
    Sobel(imageGris, gradY, CV_64F, 0, 1, 3);  // Gradient vertical
    magnitude = abs(gradX) + abs(gradY);       // Combiner les gradients

    // Normaliser la magnitude pour affichage
    Mat magnitudeNorm;
    normalize(magnitude, magnitudeNorm, 0, 255, NORM_MINMAX, CV_8UC1);

    // Rehaussement des contours en combinant avec l'image originale
    Mat contoursCouleur, imageRehaussee;
    cvtColor(magnitudeNorm, contoursCouleur, COLOR_GRAY2BGR); // Convertir les contours en couleur
    addWeighted(imageCouleur, 0.8, contoursCouleur, 0.2, 0, imageRehaussee);

    // Afficher les résultats
    imshow("Contours détectés (Magnitude)", magnitudeNorm);
    imshow("Image Rehaussée", imageRehaussee);

    // Sauvegarder les résultats dans le dossier BDD_Image
    string dossierSauvegarde = "routines/BDD_Image/";
    imwrite(dossierSauvegarde + "Brain2_Contours.jpg", magnitudeNorm);
    imwrite(dossierSauvegarde + "Brain2_Rehaussee.jpg", imageRehaussee);

    // Attendre une touche pour fermer
    waitKey(0);
    return 0;
}
