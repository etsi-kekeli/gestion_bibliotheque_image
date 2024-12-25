#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "Image.h"

int main() {
	// pour naviguer au repertoire ou se trouve le main
	std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

	// Afficher le repertoire actuel (for debugging)
	std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;

	std::string imagePath = "testImage.png";
	cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

	// Créez une instance de la classe Image
    std::cout << "Avant la création de l'objet Image" << std::endl;
    Image myImage(image);
    std::cout << "Après la création de l'objet Image" << std::endl;

    // Appel de la méthode afficherImage
    myImage.afficherImage();
	// cv::imshow("Affichage", image);
	//cv::waitKey(0);
	/*
	// Créez une instance de la classe Image
	std::cout << "Avant la création de l'objet Image" << std::endl;
    Image myImage(image);
    std::cout << "Après la création de l'objet Image" << std::endl;

    try {
        std::cout << "Avant l'appel à segmentationContour()" << std::endl;
        cv::Mat result = myImage.segmentationContour();
        std::cout << "Après l'appel à segmentationContour()" << std::endl;
        
        // Affichage du résultat
        cv::imshow("Résultat de la segmentation", result);
        cv::waitKey(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception capturée : " << e.what() << std::endl;
    }
	*/

	return 0;
}