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

    // D�finir le chemin de l'image
    std::string imagePath = "BDD_Image/lena.tif";

    // Charger l'image
    cv::Mat imageData = cv::imread(imagePath, cv::IMREAD_COLOR);

    // V�rifier si l'image est correctement charg�e
    if (imageData.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image (main) " << imagePath << std::endl;
        return -1;
    }

    // Cr�er un objet Image
    Image myImage(imageData);

    /* 
    
    // filtre moyenneur 
    
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F) / 9.0f;
  

     // filtre laplacien 
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
     0, -1,  0,
    -1,  4, -1,
     0, -1,  0); 

     // Sobel: D�tection verticale 
     cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1);

     // sobel : d�tection horizontale 
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
    int borderSize = kernel.rows / 2; // Taille de la bordure bas�e sur la taille du noyau
    cv::copyMakeBorder(imageData, imageWithBorder, borderSize, borderSize, borderSize, borderSize, cv::BORDER_REFLECT);

    // Appliquer la convolution sur l'image avec la bordure
    cv::Mat filteredImage;
    cv::filter2D(imageWithBorder, filteredImage, -1, kernel);

    // Supprimer la bordure pour obtenir une image finale de la m�me taille que l'originale
    cv::Mat result = filteredImage(cv::Rect(borderSize, borderSize, imageData.cols, imageData.rows));

    cv::imshow("Original Image", imageData); // Fen�tre pour l'image originale
    cv::imshow("Filtered Image", result);   // Fen�tre pour l'image filtr�e

     // Attendre que l'utilisateur appuie sur une touche pour fermer les fen�tres
    cv::waitKey(0); 

    return 0;

}
