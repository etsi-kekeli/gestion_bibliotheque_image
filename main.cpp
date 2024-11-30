#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

int main() {
	// pour naviguer au repertoire ou se trouve le main
	std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

	// Afficher le repertoire actuel (for debugging)
	std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;

	std::string imagePath = "testImage.png";
	cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

	// vérifier le chargement de l'image
	if (image.empty()) {
		std::cerr << "Erreur: chargement de l'image impossible " << imagePath << std::endl;
		return -1;  
	}
	cv::imshow("Affichage", image);
	cv::waitKey(0);

	return 0;
}