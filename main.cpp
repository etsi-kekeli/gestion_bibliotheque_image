#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "Image.h"
#include "Descripteur.h"
#include "Utilisateur.h"

int main()
{
    Utilisateur utilisateur;
    char choix;
    /* Code manel qui permet de créer et tester utilisateur
    do {
        std::cout << "\nMenu :\n";
        std::cout << "1. Creer un utilisateur\n";
        std::cout << "2. Modifier un utilisateur\n";
        std::cout << "3. Supprimer un utilisateur\n";
        std::cout << "4. Quitter\n";
        std::cout << "Entrez votre choix : ";
        std::cin >> choix;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choix) {
        case '1':
            utilisateur.creerUtilisateur();
            break;
        case '2':
            utilisateur.modifierUtilisateur();
            break;
        case '3':
            utilisateur.supprimerUtilisateur();
            break;
        case '4':
            std::cout << "Au revoir !\n";
            break;
        default:
            std::cout << "Choix invalide. Reessayez.\n";
        }
    } while (choix != '4');
 */

    // pour naviguer au repertoire ou se trouve le main
    // J'ai commenté la ligne car elle engendre une erreur au moment de l'execution
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
    Image myImage(carres);
    Image lenaColor(lena);
    Image carresColor(carres);
    Image culturesColor(cultures);
    Image imageMedtest(image);
    std::cout << "Apres la creation de l'objet Image" << std::endl;
    /*
    // test Convolution 2D (Manel)
    // Appliquer la convolution avec un filtre moyenneur
    std::cout << "Application du filtre moyenneur sur l'image." << std::endl;

    // Appel à la méthode convolution2D avec le filtre moyenneur
    Mat result = culturesColor.convolution2D(cultures, FilterType::MEAN);

    // Afficher l'image originale et le résultat
    cv::imshow("Image Originale", cultures);
	//lenaColor.afficherImage();
    cv::imshow("Image avec Filtre Moyenneur", result);
    cv::waitKey(0); 
    // fin test Convolution 2D.
    */

    /*
    // Tester la détection des contour (Arame)
    // Tester la détection des contours avec le gradient
    std::cout << "Détection des contours (Gradient) sur l'image." << std::endl;
    lenaColor.detectionContours(true); // true pour utiliser le gradient

    // Tester la détection des contours avec le Laplacien
    std::cout << "Détection des contours (Laplacien) sur l'image." << std::endl;
    lenaColor.detectionContours(false); // false pour utiliser le Laplacien
    // fin test détection de contour (Arame)
    */
    /*
	// Test de la méthode rehaussementContour (Achour)
    std::cout << "Test de la méthode rehaussementContour" << std::endl;

    // Exemple avec une image en niveaux de gris
    std::cout << "Test avec une image en niveaux de gris (medtest.png)" << std::endl;
    cv::imshow("Image Originale", image);
    imageMedtest.rehaussementContour();

    // Exemple avec une image en couleurs
    std::cout << "Test avec une image en couleurs (lena.tif)" << std::endl;
    cv::imshow("Image Originale", lena);
    lenaColor.rehaussementContour();
    // fin test rehaussement
    */
    /*
    // test seuillage (Amine)
    // Appliquer le seuillage avec une valeur de seuil
    int seuil = 100; // Définir la valeur du seuil
    imageMedtest.seuillage(seuil);
    lenaColor.seuillage(seuil);
    // fin test seuillage
    */
    /*
    cv::imshow("Transformée de Hough", myImage.getTableDAccumulationNormalisee(400, 360));
    cv::waitKey(0);

    cv::imshow("Image avec ligne de Hough", myImage.dessineLigneHough(carres.rows * 2 + 1, 360, 10, 100));
    cv::waitKey(0);
    // Mat carreLigneHough = myImage.dessineLigneHough(400, 360, 10, 100);

    /*
    // Appel de la méthode afficherImage
    myImage.afficherImage();
    */
    /*
    try
    {
        std::cout << "Avant l'appel a segmentation" << std::endl;

        // myImage.segmentationCouleurOuNG(image,230, 255,50, 200,0, 150);
        // lenaColor.segmentationCouleurOuNG(lena, 100, 255, 50, 200, 0, 150);
        // carresColor.segmentationCouleurOuNG(carres, 0, 255, 0, 1, 0, 1);
        cv::Mat hue = culturesColor.afficherTeinte(cultures);
        culturesColor.segmenterParTeinte(cultures, hue, 140, 180, 7);
        std::cout << "Apres l'appel a segmentation" << std::endl;

        cv::waitKey(0);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception capturee : " << e.what() << std::endl;
    }

	// Afficher l'histogramme de l'image (code Ydriss)
    imshow("Image Originale", image);
    imageMedtest.calculateAndDisplayHistogram(image);
    cv::waitKey(0);



    // Créez une instance de Descripteur et associez-la à l'image
    Descripteur *monDescripteur = new Descripteur();
    try
    {
        monDescripteur->creerDescripteur(imagePathCarres);
    }
    catch (const std::exception &e)
    {
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
    

    // suppression d'un descripteur
    try
    {
        monDescripteur->supprimerDescripteur();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception capturee lors de la suppression : " << e.what() << std::endl;
    }
    */
    return 0;
}
