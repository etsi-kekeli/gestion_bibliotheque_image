#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "Image.h"

int main() {
	// pour naviguer au repertoire ou se trouve le main
	std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

	// Afficher le repertoire actuel (for debugging)
	std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;

	std::string imagePath = "BDD_Image/medtest.png";
    std::string imagePathLena = "BDD_Image/lena.tif";
    std::string imagePathCarres = "BDD_Image/carres.tif";

	cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat lena = cv::imread(imagePathLena, cv::IMREAD_COLOR);
    cv::Mat carres = cv::imread(imagePathCarres, cv::IMREAD_COLOR);
    
	// Créez une instance de la classe Image
	std::cout << "Avant la creation de l'objet Image" << std::endl;
    Image myImage(image);
	Image lenaColor;  
	Image carresColor;
    std::cout << "Apres la creation de l'objet Image" << std::endl;
    /*
    // Appel de la méthode afficherImage
    myImage.afficherImage();
    */
    try {
        std::cout << "Avant l'appel a segmentation" << std::endl;
        
        //myImage.segmentationCouleurOuNG(image,230, 255,50, 200,0, 150);
        //lenaColor.segmentationCouleurOuNG(lena, 100, 255, 50, 200, 0, 150);
        carresColor.segmentationCouleurOuNG(carres, 0, 255, 0, 1, 0, 1);
        std::cout << "Apres l'appel a segmentation" << std::endl;
        
        cv::waitKey(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception capturee : " << e.what() << std::endl;
    }
	

	return 0;
}