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

	std::string imagePath = "BDD_Image/medtest.png";
    std::string imagePathLena = "BDD_Image/lena.tif";
    std::string imagePathCarres = "BDD_Image/carres.tif";
    std::string imagePathCultures = "BDD_Image/imageCultures.png";

	cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat lena = cv::imread(imagePathLena, cv::IMREAD_COLOR);
    cv::Mat carres = cv::imread(imagePathCarres, cv::IMREAD_COLOR);
    cv::Mat cultures = cv::imread(imagePathCultures, cv::IMREAD_COLOR);
    
	// Créez une instance de la classe Image
	std::cout << "Avant la creation de l'objet Image" << std::endl;
    Image myImage(image);
	Image lenaColor;  
	Image carresColor;
    Image culturesColor;
    std::cout << "Apres la creation de l'objet Image" << std::endl;
    /*
    // Appel de la méthode afficherImage
    myImage.afficherImage();
    */
    try {
        std::cout << "Avant l'appel a segmentation" << std::endl;
        
        //myImage.segmentationCouleurOuNG(image,230, 255,50, 200,0, 150);
        //lenaColor.segmentationCouleurOuNG(lena, 100, 255, 50, 200, 0, 150);
        //carresColor.segmentationCouleurOuNG(carres, 0, 255, 0, 1, 0, 1);
        cv::Mat hue = culturesColor.afficherTeinte(cultures);
        culturesColor.segmenterParTeinte(cultures,hue,140,180,7);
        std::cout << "Apres l'appel a segmentation" << std::endl;
        
        cv::waitKey(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception capturee : " << e.what() << std::endl;
    }

    // Créez une instance de Descripteur et associez-la à l'image
    Descripteur* monDescripteur = new Descripteur();
    try {
        monDescripteur->creerDescripteur(myImage);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception capturee lors de la creation: " << e.what() << std::endl;
    }
    /*
    // Modification d'un descripteur
    try {
        monDescripteur->modifierDescripteur();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception capturee lors de la modification : " << e.what() << std::endl;
    }
    */

    // suppression d'un descripteur
    try {
        monDescripteur->supprimerDescripteur();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception capturee lors de la suppression : " << e.what() << std::endl;
    }

	return 0;
}
