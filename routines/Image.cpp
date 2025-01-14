#include "Image.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
/**
 * Image implementation
 */
 // Constructeur par défaut
Image::Image() : data(), Format(""), Couleur(false), TauxCompression(0.0f), titreImage("")
{
}

// Constructeur avec paramètres
Image::Image(const Mat& data, const std::string& format,
    bool couleur, float tauxCompression)
    : data(data), Format(format), Couleur(couleur), TauxCompression(tauxCompression),
    titreImage("")
{
}

// Constructeur avec un paramètre si couleur retourne false = NGR, true = RGB
Image::Image(const Mat& data)
    : data(data),                   // Initialisation de 'data'
    Format(""),                   // Initialisation de 'Format'
    Couleur(data.channels() > 1), // Initialisation de 'Couleur'
    TauxCompression(1.0f),        // Valeur par défaut pour 'TauxCompression'
    titreImage("")
{
}

// Constructeur avec un parametre source
Image::Image(const std::string& source)
    : data(imread(source)),
    Format(""),
    Couleur(data.channels() > 1),
    TauxCompression(1.0f),
    source(source),
    titreImage("")
{
    // Déterminer le format à partir de l'extension du fichier
    size_t pos = source.find_last_of(".");
    if (pos != std::string::npos) {
        Format = source.substr(pos + 1);
    }
}

// Surcharge de l'opérateur d'affectation
    Image& Image::operator=(const Image& other)
    {
        if (this != &other)
        {
            // Copier les membres non-const
            titreImage = other.titreImage;
            
            const_cast<Mat&>(data) = other.data.clone();
            const_cast<std::string&>(Format) = other.Format;
            const_cast<bool&>(Couleur) = other.Couleur;
            const_cast<float&>(TauxCompression) = other.TauxCompression;
        }
        return *this;
    }





// Destructeur
Image::~Image()
{
    std::cout << "Destructeur de Image appele" << std::endl;
    // Pas besoin de libérer 'data', cv::Mat gère cela automatiquement
}

// Implémentation des getters et setters

const std::string& Image::getTitreImage() const
{
    return titreImage;
}
void Image::setTitreImage(const std::string& titre)
{
    titreImage = titre;
}

const Mat& Image::getData() const
{
    return data;
}

const std::string& Image::getFormat() const
{
    return Format;
}

bool Image::isCouleur() const
{
    return Couleur;
}

float Image::getTauxCompression() const
{
    return TauxCompression;
}

const std::string& Image::getSource() const
{
    return source;
}

// les méthodes
void Image::afficherImage() const
{
    if (!data.empty())
    {
        imshow("Affichage Image", data);
        std::cout << "Affichage de l'image" << std::endl;
        waitKey(0);
    }
    else
    {
        std::cerr << "Erreur: aucune image a afficher" << std::endl;
    }
}

// Méthode pour calculer et afficher l'histogramme (Ydriss)
void Image::calculateAndDisplayHistogram(const Mat& image)
{
    // Vérification si l'image est en couleur ou en niveaux de gris
    vector<Mat> channels;
    if (image.channels() == 3)
    {
        // Image en couleur, décomposez les canaux BGR
        split(image, channels);
    }
    else
    {
        // Image en niveaux de gris
        channels.push_back(image);
    }

    int histSize = 256;
    float range[] = { 0, 256 };     // intensités
    const float* histRange = { range };
    int hist_w = 512, hist_h = 400; // Taille de l'image de l'histogramme
    int bin_w = cvRound((double)hist_w / histSize);

    for (size_t i = 0; i < channels.size(); i++)
    {
        Mat histogram;
        calcHist(&channels[i], 1, 0, Mat(), histogram, 1, &histSize, &histRange);
        normalize(histogram, histogram, 0, hist_h, NORM_MINMAX);

        // Dessin de l'histogramme
        Mat histImage(hist_h + 50, hist_w + 50, CV_8UC3, Scalar(255, 255, 255));
        Scalar color = (channels.size() == 1)
            ? Scalar(0, 0, 0) // Noir pour niveaux de gris
            : (i == 0 ? Scalar(255, 0, 0) : (i == 1 ? Scalar(0, 255, 0) : Scalar(0, 0, 255)));

        // Dessiner l'histogramme
        for (int j = 0; j < histSize; j++)
        {
            line(histImage, Point(bin_w * j + 25, hist_h + 25),
                Point(bin_w * j + 25, hist_h + 25 - cvRound(histogram.at<float>(j))),
                color, 2);
        }

        // noms des axes
        putText(histImage, "Intensite", Point(hist_w / 2, hist_h + 45), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2); // Axe des X
        putText(histImage, "Frequence", Point(5, hist_h / 2), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2); // Axe des Y (vertical)


        line(histImage, Point(25, 25), Point(25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe Y
        line(histImage, Point(25, hist_h + 25), Point(hist_w + 25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe X

        // Afficher l'histogramme
        string windowName = (channels.size() == 1)
            ? "Histogramme Niveaux de Gris"
            : (i == 0 ? "Histogramme Bleu" : (i == 1 ? "Histogramme Vert" : "Histogramme Rouge"));
        imshow(windowName, histImage);
    }
}

// Méthode pour appliquer une convolution 2D (Manel)
//options pour type filtres : MEAN, SOBEL_X, SOBEL_Y, LAPLACIAN, GAUSSIAN
Mat Image::convolution2D(const Mat& src, FilterType filterType)
{
    Mat kernel;
    switch (filterType) {
    case FilterType::MEAN:
        kernel = (Mat_<float>(3, 3) << 1 / 9.0, 1 / 9.0, 1 / 9.0,
            1 / 9.0, 1 / 9.0, 1 / 9.0,
            1 / 9.0, 1 / 9.0, 1 / 9.0);
        break;
    case FilterType::SOBEL_X:
        kernel = (Mat_<float>(3, 3) << -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1);
        break;
    case FilterType::SOBEL_Y:
        kernel = (Mat_<float>(3, 3) << -1, -2, -1,
            0, 0, 0,
            1, 2, 1);
        break;
    case FilterType::LAPLACIAN:
        kernel = (Mat_<float>(3, 3) << 0, 1, 0,
            1, -4, 1,
            0, 1, 0);
        break;
    case FilterType::GAUSSIAN:
        kernel = (Mat_<float>(3, 3) << 1 / 16.0, 2 / 16.0, 1 / 16.0,
            2 / 16.0, 4 / 16.0, 2 / 16.0,
            1 / 16.0, 2 / 16.0, 1 / 16.0);
        break;
    }

    Mat dst;
    if (src.channels() == 3) {
        // Pour les images en couleur
        std::vector<Mat> channels(3);
        split(src, channels); // Séparer les canaux R, G et B

        for (int c = 0; c < channels.size(); ++c) {
            Mat channelResult = Mat::zeros(src.size(), CV_32F);
            for (int i = 0; i < src.rows; ++i) {
                for (int j = 0; j < src.cols; ++j) {
                    float sum = 0.0;
                    for (int m = -kernel.rows / 2; m <= kernel.rows / 2; ++m) {
                        for (int n = -kernel.cols / 2; n <= kernel.cols / 2; ++n) {
                            int imagex = j + n;
                            int imagey = i + m;

                            // Réflexion des pixels de bord
                            if (imagex < 0) imagex = -imagex;
                            if (imagex >= src.cols) imagex = src.cols - (imagex - src.cols + 1);
                            if (imagey < 0) imagey = -imagey;
                            if (imagey >= src.rows) imagey = src.rows - (imagey - src.rows + 1);

                            sum += channels[c].at<uchar>(imagey, imagex) * kernel.at<float>(m + kernel.rows / 2, n + kernel.cols / 2);
                        }
                    }
                    channelResult.at<float>(i, j) = sum;
                }
            }
            channels[c] = channelResult; // Stocker le résultat dans le canal
        }

        merge(channels, dst); // Fusionner les canaux traités
    }
    else {
        // Pour les images en niveaux de gris
        dst = Mat::zeros(src.size(), CV_32F);
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < src.cols; ++j) {
                float sum = 0.0;
                for (int m = -kernel.rows / 2; m <= kernel.rows / 2; ++m) {
                    for (int n = -kernel.cols / 2; n <= kernel.cols / 2; ++n) {
                        int imagex = j + n;
                        int imagey = i + m;

                        // Réflexion des pixels de bord
                        if (imagex < 0) imagex = -imagex;
                        if (imagex >= src.cols) imagex = src.cols - (imagex - src.cols + 1);
                        if (imagey < 0) imagey = -imagey;
                        if (imagey >= src.rows) imagey = src.rows - (imagey - src.rows + 1);

                        sum += static_cast<float>(src.at<uchar>(imagey, imagex)) * kernel.at<float>(m + kernel.rows / 2, n + kernel.cols / 2);
                    }
                }
                dst.at<float>(i, j) = sum;
            }
        }
    }

    return dst;
}

// Méthode pour détecter les contours (True pour Gradient False pour Laplacien (Arame)
void Image::detectionContours(bool useGradient)
{
    Mat grayImage;
    if (data.channels() > 1)
        cvtColor(data, grayImage, COLOR_BGR2GRAY);
    else
        grayImage = data.clone();

    Mat result;

    if (useGradient)
    {
        Mat gradX = convolution2D(grayImage, FilterType::SOBEL_X);
        Mat gradY = convolution2D(grayImage, FilterType::SOBEL_Y);
        result = calculateMagnitude(gradX, gradY);
    }
    else
    {
        result = convolution2D(grayImage, FilterType::LAPLACIAN);
        result = abs(result);
    }

    normalize(result, result, 0, 255, NORM_MINMAX);
    result.convertTo(result, CV_8U);

    imshow(useGradient ? "Contours (Gradient)" : "Contours (Laplacien)", result);
    waitKey(0);
}

// Méthode pour calculer Magnitude afin de detecter le contour par le Gradient (Arame)
Mat Image::calculateMagnitude(const Mat& gradX, const Mat& gradY)
{
    Mat magnitude = Mat::zeros(gradX.size(), CV_32F);
    for (int i = 0; i < gradX.rows; ++i)
    {
        for (int j = 0; j < gradX.cols; ++j)
        {
            float gx = gradX.at<float>(i, j);
            float gy = gradY.at<float>(i, j);
            magnitude.at<float>(i, j) = std::sqrt(gx * gx + gy * gy);
        }
    }
    return magnitude;
}

// Méthode pour rehausser les contours de l'image (Achour)
void Image::rehaussementContour() {
    if (data.empty()) {
        std::cerr << "Erreur : L'image source est vide !" << std::endl;
        return;
    }

    Mat originalImage = data.clone();
    Mat contours;

    // Détection des contours en utilisant la méthode detectionContours
    detectionContours(false); // Utilisation de l'option Gradient
    contours = data.clone(); // Les contours devraient être écrits dans 'data' par detectionContours

    if (contours.empty()) {
        std::cerr << "Erreur : La détection de contours a échoué !" << std::endl;
        return;
    }

    // Vérification et harmonisation des types
    if (originalImage.channels() == 3 && contours.channels() == 1) {
        cvtColor(contours, contours, cv::COLOR_GRAY2BGR); // Convertir en BGR si nécessaire
    }

    if (originalImage.type() != contours.type()) {
        contours.convertTo(contours, originalImage.type()); // Harmoniser les types
    }

    try {
        // Ajout des contours à l'image originale
        cv::add(originalImage, contours, originalImage, cv::Mat(), originalImage.type());

        // Afficher l'image avec contours rehaussés
        cv::imshow("Image avec contours rehaussés", originalImage);
        cv::waitKey(0);
    }
    catch (const cv::Exception& e) {
        std::cerr << "Erreur lors de l'ajout des contours : " << e.what() << std::endl;
    }
}

// méthode pour seuiller l'image (Amine)
void Image::seuillage(int seuil)
{
    if (data.empty()) {
        std::cerr << "Erreur : Les données de l'image sont vides." << std::endl;
        return;
    }

    cv::Mat imageNG;

    // Convertir l'image en niveaux de gris
    if (data.channels() == 3) {
        cv::cvtColor(data, imageNG, cv::COLOR_BGR2GRAY); // Image en couleur, conversion en NG
    }
    else {
        imageNG = data.clone(); // Image déjà en NG
    }

    // Créer une copie pour stocker l'image seuillée
    cv::Mat imageSeuillee = imageNG.clone();

    // Appliquer le seuillage sur l'image en niveaux de gris
    for (int i = 0; i < imageNG.rows; ++i) {
        for (int j = 0; j < imageNG.cols; ++j) {
            uchar pixelValue = imageNG.at<uchar>(i, j);
            imageSeuillee.at<uchar>(i, j) = (pixelValue > seuil) ? 255 : 0;
        }
    }

    // Afficher l'image originale et l'image seuillée
    cv::imshow("Image Originale", data);
    cv::imshow("Image Seuillée", imageSeuillee);

    // Attente pour l'utilisateur
    cv::waitKey(0);
}


int mapRho(double rho, int n, double rhomax)
{
    return cvRound(n * (rho + rhomax) / (2 * rhomax + 1));
}

Mat Image::transformeeHough(int nRho, int nTheta)
{
    Mat contours;
    Canny(data, contours, 50, 200);

    vector<Point> pointsContours;
    findNonZero(contours, pointsContours);

    Mat tableDAccumulation = Mat::zeros(nRho, nTheta, CV_16U);
    double pasAngulaire = CV_PI / nTheta;
    int rhomax = cvCeil(sqrt(pow(data.rows, 2) + pow(data.cols, 2)));

    for (int i = 0; i < pointsContours.size(); i++)
    {
        int x = pointsContours[i].x;
        int y = pointsContours[i].y;
        for (int k = 0; k < nTheta; k++)
        {
            double angle = pasAngulaire * k;
            double rho = x * cos(angle) + y * sin(angle);
            tableDAccumulation.at<unsigned short>(mapRho(rho, nRho, rhomax), k) += 1;
        }
    }

    return tableDAccumulation;
}

Mat Image::getTableDAccumulationNormalisee(int nRho, int nTheta)
{
    Mat table = transformeeHough(nRho, nTheta);
    Mat tableNormaliseeTemp;
    normalize(table, tableNormaliseeTemp, 0, 255, cv::NORM_MINMAX);
    cv::Mat tableNormalisee;
    tableNormaliseeTemp.convertTo(tableNormalisee, CV_8U);
    return tableNormalisee;
}

vector<pair<float, float>> getHoughMaxima(Mat tableDAccumulation, double rhomax, int tailleVoisinage, int seuil)
{
    Mat accDilate;
    dilate(tableDAccumulation, accDilate, Mat::ones(tailleVoisinage, tailleVoisinage, CV_32F));
    Mat maximaLocaux = (tableDAccumulation == accDilate);

    Mat accumalateurSeuille;
    threshold(tableDAccumulation, accumalateurSeuille, seuil, 255, THRESH_BINARY);

    Mat accSeuilleBin;
    accumalateurSeuille.convertTo(accSeuilleBin, CV_8U);

    // cout << "Seuillage tableDAccumulation fait" << endl;
    // cout << maximaLocaux.type() << endl;
    // cout << accumalateurSeuille.type() << endl;

    Mat parametresDInterets;
    bitwise_and(maximaLocaux, accSeuilleBin, parametresDInterets);

    // imshow("Param", parametresDInterets);
    // waitKey(0);

    vector<pair<float, float>> resultat;
    vector<Point> tmp;
    findNonZero(parametresDInterets, tmp);

    double pasAngulaire = CV_PI / parametresDInterets.cols;

    for (int i = 0; i < tmp.size(); i++)
    {
        resultat.push_back({ tmp[i].y * (2 * rhomax + 1) / parametresDInterets.rows - rhomax, tmp[i].x * pasAngulaire });
    }

    return resultat;
}

void _dessineLigneHough(Mat& image, float rho, float theta, Scalar couleur, int epaisseur)
{
    double pente = tan(theta + CV_PI / 2);

    // Points d'intersection avec les bords
    Point pt1, pt2;
    double y0 = rho * sin(theta);
    double x0 = rho * cos(theta);

    double ordonneeOrigine = y0 - pente * x0;

    // cout << "Rho " << rho << " theta " << theta / CV_PI * 180 << endl;

    // cout << "Pente " << pente << " ordonnée à l'origine" << ordonneeOrigine << endl;

    if (abs(theta) < 2e-2 || abs(theta) > CV_PI - 2e-2)
    {
        pt1.y = 0;
        pt1.x = cvRound(abs(x0));
        pt2.y = image.rows;
        pt2.x = cvRound(abs(x0));
    }
    else
    {
        pt1.x = 0;
        pt1.y = cvRound(ordonneeOrigine);
        pt2.x = image.cols;
        pt2.y = cvRound(pente * image.cols + ordonneeOrigine);
    }

    // cout << pt1 << pt2 << endl;

    line(image, pt1, pt2, couleur, epaisseur, cv::LINE_AA);
}

Mat Image::dessineLigneHough(int nRho, int nTheta, int tailleVoisinage, int seuil, Scalar couleur, int epaisseur)
{
    Mat votes = transformeeHough(nRho, nTheta);
    int rhomax = cvCeil(sqrt(pow(data.rows, 2) + pow(data.cols, 2)));
    vector<pair<float, float>> parametres = getHoughMaxima(votes, rhomax, tailleVoisinage, seuil);

    Mat resultat = data.clone();

    for (const auto param : parametres)
    {
        float rho = param.first;
        float theta = param.second;
        _dessineLigneHough(resultat, rho, theta, couleur, epaisseur);
    }

    return resultat;
}

// segmentation couleur ou noir et blanc
cv::Mat Image::segmentationCouleurOuNG(const cv::Mat& imageOriginale,
    uchar seuilBasR, uchar seuilHautR,
    uchar seuilBasG, uchar seuilHautG,
    uchar seuilBasB, uchar seuilHautB)
{

    // Créer une image binaire pour le masque
    cv::Mat masque = cv::Mat::zeros(imageOriginale.size(), CV_8U);

    // Vérifier si l'image est en couleur (RGB) ou en niveaux de gris (NG)
    if (imageOriginale.channels() == 3)
    { // Image RGB
        for (int y = 0; y < imageOriginale.rows; y++)
        {
            for (int x = 0; x < imageOriginale.cols; x++)
            {
                // Obtenir les valeurs des canaux R, G et B pour l'image RGB
                cv::Vec3b pixel = imageOriginale.at<cv::Vec3b>(y, x); // Le pixel est un vecteur avec 3 valeurs : R, G, B
                uchar valeurR = pixel[2];                             // Canal Rouge (index 2)
                uchar valeurG = pixel[1];                             // Canal Vert (index 1)
                uchar valeurB = pixel[0];                             // Canal Bleu (index 0)

                // Vérifier si la couleur du pixel est dans les intervalles définis
                if (valeurR >= seuilBasR && valeurR <= seuilHautR &&
                    valeurG >= seuilBasG && valeurG <= seuilHautG &&
                    valeurB >= seuilBasB && valeurB <= seuilHautB)
                {
                    masque.at<uchar>(y, x) = 255; // Pixel dans l'intervalle
                }
            }
        }
    }
    else if (imageOriginale.channels() == 1)
    { // Image en niveaux de gris
        for (int y = 0; y < imageOriginale.rows; y++)
        {
            for (int x = 0; x < imageOriginale.cols; x++)
            {
                uchar valeurNG = imageOriginale.at<uchar>(y, x); // Valeur du pixel en NG

                // Utiliser uniquement le seuil rouge pour le seuillage
                if (valeurNG >= seuilBasR && valeurNG <= seuilHautR)
                {
                    masque.at<uchar>(y, x) = 255; // Pixel au-dessus du seuil rouge
                }
            }
        }
    }

    // Appliquer le masque à l'image originale pour obtenir l'image segmentée
    cv::Mat imageSegmentee;
    imageOriginale.copyTo(imageSegmentee, masque);

    // Afficher le résultat
    cv::imshow("image segmentee", imageSegmentee);
    cv::waitKey(0); // Attendre une touche pour fermer la fenêtre

    return imageSegmentee;
}

// affichage de l'image en mode teinte
cv::Mat Image::afficherTeinte(const cv::Mat& image)
{
    // Convertir l'image de BGR à HSV
    cv::Mat imageHSV;
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);

    // Extraire le canal de teinte
    std::vector<cv::Mat> channels;
    cv::split(imageHSV, channels);
    cv::Mat hue = channels[0];

    // Appliquer une colormap à l'image de teinte
    cv::Mat hueColoree;
    cv::applyColorMap(hue, hueColoree, cv::COLORMAP_HSV);

    // Afficher l'image de teinte avec le codage en couleur
    cv::imshow("Teinte (Hue)", hueColoree);
    cv::waitKey(0);

    return hue;
}

void Image::segmenterParTeinte(const cv::Mat& image, const cv::Mat& hue, int seuilBas, int seuilHaut, int taillekernel)
{
    cv::Mat masque = cv::Mat::zeros(hue.size(), CV_8U);

    for (int y = 0; y < hue.rows; y++)
    {
        for (int x = 0; x < hue.cols; x++)
        {
            uchar valeurHue = hue.at<uchar>(y, x);
            if (valeurHue >= seuilBas && valeurHue <= seuilHaut)
            {
                masque.at<uchar>(y, x) = 255;
            }
        }
    }
    // Appliquer la morphologie mathématique
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(taillekernel, taillekernel));
    cv::Mat masqueMorpho;
    cv::morphologyEx(masque, masqueMorpho, cv::MORPH_OPEN, element);

    // Appliquer le masque à l'image originale
    cv::Mat imageSegmentee;
    image.copyTo(imageSegmentee, masqueMorpho);

    // Afficher l'image segmentée
    cv::imshow("Image segmentee", imageSegmentee);
    cv::waitKey(0);
}
