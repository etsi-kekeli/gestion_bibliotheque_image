#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "Image.h"
#include "Descripteur.h"


int main() {

    // pour naviguer au repertoire ou se trouve le main
    std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

    // Afficher le repertoire actuel (for debugging)
    std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;

    // Définir le chemin de l'image
    std::string imagePath = "BDD_Image/lena.tif";

    // Charger l'image
    cv::Mat imageData = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Vérifier si l'image est correctement chargée
    if (imageData.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image (main) " << imagePath << std::endl;
        return -1;
    }

    // Créer un objet Image
    Image myImage(imageData);

    /* 
    
    // filtre moyenneur 
    
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F) / 9.0f;
  

     // filtre laplacien 
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
     0, -1,  0,
    -1,  4, -1,
     0, -1,  0); 

     // Sobel: Détection verticale 
     cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1);

     // sobel : détection horizontale 
     cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1);

    // filtre gaussien 
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
    1 / 16.0f, 2 / 16.0f, 1 / 16.0f,
    2 / 16.0f, 4 / 16.0f, 2 / 16.0f,
    1 / 16.0f, 2 / 16.0f, 1 / 16.0f);

*/
  
// filtre gaussien 
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        1 / 16.0f, 2 / 16.0f, 1 / 16.0f,
        2 / 16.0f, 4 / 16.0f, 2 / 16.0f,
        1 / 16.0f, 2 / 16.0f, 1 / 16.0f);


    // Ajouter une bordure miroir autour de l'image
    cv::Mat imageWithBorder;
    int borderSize = kernel.rows / 2; // Taille de la bordure basée sur la taille du noyau
    cv::copyMakeBorder(imageData, imageWithBorder, borderSize, borderSize, borderSize, borderSize, cv::BORDER_REFLECT);

    // Appliquer la convolution sur l'image avec la bordure
    cv::Mat filteredImage;
    cv::filter2D(imageWithBorder, filteredImage, -1, kernel);

    // Supprimer la bordure pour obtenir une image finale de la même taille que l'originale
    cv::Mat result = filteredImage(cv::Rect(borderSize, borderSize, imageData.cols, imageData.rows));

    cv::imshow("Original Image", imageData); // Fenêtre pour l'image originale
    cv::imshow("Filtered Image", result);   // Fenêtre pour l'image filtrée

     // Attendre que l'utilisateur appuie sur une touche pour fermer les fenêtres
    cv::waitKey(0); 

    return 0;

}
