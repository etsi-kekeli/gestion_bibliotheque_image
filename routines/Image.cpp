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

 // histograme (Ydriss)
Mat Image::calculateAndDisplayHistogram(const std::vector<Mat>& channels)
{
    int histSize = 256;
    float range[] = { 0, 256 };  // Intensité des pixels
    const float* histRange = { range };
    int hist_w = 512, hist_h = 400;  // Taille de l'histogramme
    int bin_w = cvRound((double)hist_w / histSize);

    // Créer une image vide pour afficher l'histogramme
    Mat histImage(hist_h + 50, hist_w + 50, CV_8UC3, Scalar(255, 255, 255));

    // Dessiner l'histogramme pour chaque canal
    Scalar color;
    for (int i = 0; i < channels.size(); i++) {
        Mat histogram;
        calcHist(&channels[i], 1, 0, Mat(), histogram, 1, &histSize, &histRange);
        normalize(histogram, histogram, 0, hist_h, NORM_MINMAX);

        // Définir la couleur du canal
        if (i == 0) color = Scalar(255, 0, 0);  // Bleu
        else if (i == 1) color = Scalar(0, 255, 0);  // Vert
        else if (i == 2) color = Scalar(0, 0, 255);  // Rouge

        // Dessiner les lignes de l'histogramme pour chaque canal
        for (int j = 0; j < histSize; j++) {
            line(histImage, Point(bin_w * j + 25, hist_h + 25),
                 Point(bin_w * j + 25, hist_h + 25 - cvRound(histogram.at<float>(j))),
                 color, 2);
        }
    }

    // Ajouter des noms aux axes
    putText(histImage, "Intensite", Point(hist_w / 2, hist_h + 45), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(histImage, "Frequence", Point(5, hist_h / 2), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);

    // Dessiner les axes X et Y
    line(histImage, Point(25, 25), Point(25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe Y
    line(histImage, Point(25, hist_h + 25), Point(hist_w + 25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe X

    return histImage;
}

Mat Image::calculateAndDisplayHistogram(const Mat& image, const std::string& channel)
{
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histogram;
    calcHist(&image, 1, 0, Mat(), histogram, 1, &histSize, &histRange);
    normalize(histogram, histogram, 0, hist_h, NORM_MINMAX);

    Mat histImage(hist_h + 50, hist_w + 50, CV_8UC3, Scalar(255, 255, 255));

    Scalar color = (channel == "Bleu") ? Scalar(255, 0, 0) :
                       (channel == "Vert") ? Scalar(0, 255, 0) :
                       (channel == "Rouge") ? Scalar(0, 0, 255) :
                       Scalar(0, 0, 0);  // Par défaut, noir si autre canal non spécifié.

    // Dessiner l'histogramme pour le canal spécifique
    for (int j = 0; j < histSize; j++) {
        line(histImage, Point(bin_w * j + 25, hist_h + 25),
             Point(bin_w * j + 25, hist_h + 25 - cvRound(histogram.at<float>(j))),
             color, 2);
    }

    // Ajouter des noms aux axes
    putText(histImage, "Intensite", Point(hist_w / 2, hist_h + 45), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(histImage, "Frequence", Point(5, hist_h / 2), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);

    // Dessiner les axes X et Y
    line(histImage, Point(25, 25), Point(25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe Y
    line(histImage, Point(25, hist_h + 25), Point(hist_w + 25, hist_h + 25), Scalar(0, 0, 0), 2); // Axe X

    return histImage;
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

// Méthode pour détecter les contours (True pour Gradient False pour Laplacien (Arame))
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

    data = result.clone(); // Mettre à jour la matrice interne `data` avec le résultat

    //imshow(useGradient ? "Contours (Gradient)" : "Contours (Laplacien)", result);
    //waitKey(0);
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
// Méthode pour rehausser les contours d'une image donnée
Mat Image::rehaussementContour(const cv::Mat& inputImage, bool gradOrLap) {
    if (inputImage.empty()) {
        std::cerr << "Erreur : L'image source est vide !" << std::endl;
        return cv::Mat();
    }
    cv::Mat originalImage = inputImage.clone();
    cv::Mat contours;

    // Détection des contours en utilisant la méthode detectionContours
    Image tempImage(inputImage.clone());
    tempImage.detectionContours(gradOrLap);
    contours = tempImage.getData();

    if (contours.empty()) {
        std::cerr << "Erreur : La détection de contours a échoué !" << std::endl;
        return cv::Mat();
    }

    // Vérification et harmonisation des types
    if (originalImage.channels() == 3 && contours.channels() == 1) {
        cv::cvtColor(contours, contours, cv::COLOR_GRAY2BGR);
    }

    // Convertir les images en type float pour la multiplication
    cv::Mat originalreel, contoursreel, resultreel;
    originalImage.convertTo(originalreel, CV_32F, 1.0/255.0);
    contours.convertTo(contoursreel, CV_32F, 1.0/255.0);

    try {
        // Multiplication du masque de contours avec l'image originale
        cv::multiply(originalreel, contoursreel, resultreel);

        // Reconversion en type original (généralement CV_8U)
        cv::Mat resultMasked;
        resultreel.convertTo(resultMasked, originalImage.type(), 255.0);

        // Ajout du résultat à l'image originale
        cv::add(originalImage, resultMasked, originalImage, cv::Mat(), originalImage.type());
    }
    catch (const cv::Exception& e) {
        std::cerr << "Erreur lors du traitement : " << e.what() << std::endl;
        return cv::Mat();
    }

    return originalImage;
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
    } else {
        imageNG = data.clone(); // Image déjà en NG
    }

    // Appliquer le seuillage
    for (int i = 0; i < imageNG.rows; ++i) {
        for (int j = 0; j < imageNG.cols; ++j) {
            uchar pixelValue = imageNG.at<uchar>(i, j);
            imageNG.at<uchar>(i, j) = (pixelValue > seuil) ? 255 : 0;
        }
    }

    // Mettre à jour `data` pour qu'il contienne l'image seuillée
    data = imageNG.clone();

    // Afficher l'image originale et l'image seuillée
    //cv::imshow("Image Originale", data);
   // cv::imshow("Image Seuillée", imageSeuillee);

    // Attente pour l'utilisateur
    //cv::waitKey(0);
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

    vector<pair<unsigned short, Point>> tmp2;
    for (int i = 0; i < tmp.size(); i++) {
        Point p = tmp[i];
        tmp2.push_back( make_pair( tableDAccumulation.at<unsigned short>(p.y, p.x), p) );
    }

    sort(tmp2.begin(), tmp2.end(),  [](const auto &a, const auto &b) {
                                            return a.first > b.first;
                                    });

    size_t nbre_limite = 10;
    size_t s = (tmp2.size() < nbre_limite) ? tmp2.size() : nbre_limite;

    double pasAngulaire = CV_PI / parametresDInterets.cols;

    for (int i = 0; i < s; i++)
    {
        resultat.push_back({ tmp2[i].second.y * (2 * rhomax + 1) / parametresDInterets.rows - rhomax, tmp2[i].second.x * pasAngulaire });
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
    vector<pair<float, float>> parametres = getHoughMaxima(votes, rhomax, tailleVoisinage, (int) (0.1 * rhomax));

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
Mat Image::segmentationCouleurOuNG(const cv::Mat& imageOriginale,
                                   uchar seuilBasR, uchar seuilHautR,
                                   uchar seuilBasG, uchar seuilHautG,
                                   uchar seuilBasB, uchar seuilHautB)
{
    cv::Mat imageSegmentee = imageOriginale.clone();

    if (imageOriginale.channels() == 3)
    {
        for (int y = 0; y < imageOriginale.rows; y++)
        {
            for (int x = 0; x < imageOriginale.cols; x++)
            {
                cv::Vec3b pixel = imageOriginale.at<cv::Vec3b>(y, x);
                // Si les seuils G et B sont à 0, on ne vérifie que le canal R
                if (seuilBasG == 0 && seuilHautG == 0 && seuilBasB == 0 && seuilHautB == 0)
                {
                    if (!(pixel[2] >= seuilBasR && pixel[2] <= seuilHautR))
                    {
                        imageSegmentee.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                    }
                }
                else
                {
                    if (!(pixel[2] >= seuilBasR && pixel[2] <= seuilHautR &&
                          pixel[1] >= seuilBasG && pixel[1] <= seuilHautG &&
                          pixel[0] >= seuilBasB && pixel[0] <= seuilHautB))
                    {
                        imageSegmentee.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                    }
                }
            }
        }
    }
    else if (imageOriginale.channels() == 1)
    {
        for (int y = 0; y < imageOriginale.rows; y++)
        {
            for (int x = 0; x < imageOriginale.cols; x++)
            {
                uchar valeurNG = imageOriginale.at<uchar>(y, x);
                if (!(valeurNG >= seuilBasR && valeurNG <= seuilHautR))
                {
                    imageSegmentee.at<uchar>(y, x) = 0;
                }
            }
        }
    }

    return imageSegmentee;
}

// affichage de l'image en mode teinte
cv::Mat Image::afficherTeinte(const cv::Mat& image)
{
    cv::Mat imageHSV;
    if (image.channels() == 1) {
        // If the image is grayscale, convert it to BGR first
        cv::cvtColor(image, imageHSV, cv::COLOR_GRAY2BGR);
        cv::cvtColor(imageHSV, imageHSV, cv::COLOR_BGR2HSV);
    } else {
        // If the image is already in color, convert directly to HSV
        cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
    }

    // Extract the hue channel
    std::vector<cv::Mat> channels;
    cv::split(imageHSV, channels);
    cv::Mat hue = channels[0];

    return hue;
}

cv::Mat Image::segmenterParTeinte(const cv::Mat& image, const cv::Mat& hue, int seuilBas, int seuilHaut, int taillekernel)
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

    return imageSegmentee;

    // Afficher l'image segmentée
   // cv::imshow("Image segmentee", imageSegmentee);
    //cv::waitKey(0);
}
