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

   /* // Définir le noyau de convolution (filtre moyen 3x3)
   cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        1 / 9.0f, 1 / 9.0f, 1 / 9.0f,
        1 / 9.0f, 1 / 9.0f, 1 / 9.0f,
        1 / 9.0f, 1 / 9.0f, 1 / 9.0f);
    */
   /* // Définir le noyau de convolution (filtre moyen 3x3)
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F) / 9.0f;
   
    // Définir un noyau de convolution pour un filtre gaussien 3x3 avec sigma = 1.0
    cv::Mat kernel = cv::getGaussianKernel(3, 1.0, CV_32F) * cv::getGaussianKernel(3, 1.0, CV_32F).t();
     */
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);


    
    // Appliquer le filtre de convolution
    myImage.appliquerFiltreConvolution(kernel);

    return 0;
}
