#include "processingwindow.h"
#include "ui_processingwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QGraphicsItem>
#include "../routines/Image.h"
#include <vector>

ProcessingWindow::ProcessingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessingWindow)
    , sceneOriginal(nullptr)
    , sceneResult(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Traitement des images");
}

ProcessingWindow::~ProcessingWindow()
{
    delete sceneOriginal;
    delete sceneResult;
    delete ui;
}

void ProcessingWindow::on_SeuilDial_valueChanged(int value)
{
    ui->SeuilValLabel->setText(QString::number(value));

    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée");
        return;
    }

    try {
        // Créer une copie de l'image originale
        cv::Mat imageTraitement = originalImage.clone();

        // Créer un objet Image
        Image image(imageTraitement);

        // Appliquer le seuillage
        image.seuillage(value);

        // Récupérer l'image seuillée
        cv::Mat imageSeuillée = image.getData();

        // Afficher l'image seuillée dans la vue
        displayImage(imageSeuillée, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::bad_alloc&) {
        QMessageBox::critical(this, "Erreur", "Mémoire insuffisante pour le traitement");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", QString("Erreur: %1").arg(e.what()));
    }

}


void ProcessingWindow::on_RestValRGBSeuilButton_clicked()
{
    ui->SeuilMinRVal->setValue(0);
    ui->SeuilMinVVal->setValue(0);
    ui->SeuilMinBVal->setValue(0);
    ui->SeuilMaxRVal->setValue(0);
    ui->SeuilMaxVVal->setValue(0);
    ui->SeuilMaxBVal->setValue(0);
    ui->SeuilMinHSV->setValue(0);
    ui->SeuilMaxHSV->setValue(0);
    ui->tailleNoyau->setValue(1);
    ui->SeuilDial->setValue(0);

}

void ProcessingWindow::on_actionOuvrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir une image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.pgm *.tif)"));
    if (fileName.isEmpty())
        return;

    // Clear previous image data
    originalImage.release();
    if (sceneOriginal) {
        delete sceneOriginal;
        sceneOriginal = nullptr;
    }
    if (sceneResult) {
        delete sceneResult;
        sceneResult = nullptr;
    }


    // Charger l'image avec OpenCV
    cv::Mat cvImage = cv::imread(fileName.toStdString(), cv::IMREAD_UNCHANGED);
    if (cvImage.empty())
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image."));
        return;
    }

    // Vérification et conversion si nécessaire
    if (cvImage.channels() == 3)
    {
        qDebug() << "Image couleur BGR détectée (3 canaux).";
    }
    else if (cvImage.channels() == 4)
    {
        qDebug() << "Image couleur avec transparence détectée (4 canaux).";
    }
    else if (cvImage.channels() == 1)
    {
        qDebug() << "Image en niveaux de gris détectée (1 canal).";
    }
    else
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Format d'image non pris en charge."));
        return;
    }

    // Si image PNG mal interprétée en RGB mais est en réalité NG
    if (cvImage.channels() == 3 || cvImage.channels() == 4)
    {
        bool isGrayscale = true;
        for (int row = 0; row < cvImage.rows; ++row)
        {
            for (int col = 0; col < cvImage.cols; ++col)
            {
                cv::Vec3b pixel = cvImage.at<cv::Vec3b>(row, col);
                if (pixel[0] != pixel[1] || pixel[1] != pixel[2])
                {
                    isGrayscale = false;
                    break;
                }
            }
            if (!isGrayscale)
                break;
        }

        if (isGrayscale)
        {
            qDebug() << "Image RGB détectée mais identifiée comme niveaux de gris.";
            cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2GRAY); // Convertir en NG
        }
    }

    Image myImage(cvImage);
    myImage.setTitreImage(fileName.toStdString());


    originalImage = cvImage; // Stocker l'image dans la variable membre
    // Conversion en QImage
    QImage qImage;
    if (cvImage.channels() == 1)
    {
        qDebug() << "image en niveaux de gris.";
        qImage = QImage(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_Grayscale8);

        // Désactiver les contrôles pour les canaux inutilisés
        ui->SeuilMinVVal->setEnabled(false);
        ui->SeuilMinBVal->setEnabled(false);
        ui->SeuilMaxVVal->setEnabled(false);
        ui->SeuilMaxBVal->setEnabled(false);
    }
    else if (cvImage.channels() == 3)
    {
        qDebug() << "Image finale en couleur (RGB).";
        qImage = QImage(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888).rgbSwapped();

        // Activer les contrôles pour tous les canaux
        ui->SeuilMinVVal->setEnabled(true);
        ui->SeuilMinBVal->setEnabled(true);
        ui->SeuilMaxVVal->setEnabled(true);
        ui->SeuilMaxBVal->setEnabled(true);
    }
    else if (cvImage.channels() == 4)
    {
        qDebug() << "Image finale en couleur avec transparence (RGBA).";
        qImage = QImage(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGBA8888);

        // Activer les contrôles pour tous les canaux
        ui->SeuilMinVVal->setEnabled(true);
        ui->SeuilMinBVal->setEnabled(true);
        ui->SeuilMaxVVal->setEnabled(true);
        ui->SeuilMaxBVal->setEnabled(true);
    }

    // Affichage dans le GraphicsView
    QGraphicsScene* sceneOrigin = new QGraphicsScene(this);
    sceneOrigin->addPixmap(QPixmap::fromImage(qImage));
    ui->ImageOriginalegraphicsView->setScene(sceneOrigin);
    ui->ImageOriginalegraphicsView->fitInView(sceneOrigin->sceneRect(), Qt::KeepAspectRatio);
    // vider l'affichage de l'image resultante
    // Vider l'affichage de l'image résultante
    if (ui->ImageResultatgraphicsView->scene() != nullptr) {
        delete ui->ImageResultatgraphicsView->scene();
    }
    ui->ImageResultatgraphicsView->setScene(new QGraphicsScene(this));
    ui->ImageResultatgraphicsView->update();
    ProcessingWindow::on_RestValRGBSeuilButton_clicked();
}


void ProcessingWindow::on_actionSortir_triggered()
{
    this->close();
}


void ProcessingWindow::on_actionSauvegarder_triggered()
{

}

QImage ProcessingWindow::matToQImage(const cv::Mat& mat)
{
    if (mat.empty())
        return QImage();

    try {
        QImage image;

        switch (mat.channels()) {
        case 1: {
            // Image en niveaux de gris
            image = QImage(mat.cols, mat.rows, QImage::Format_Grayscale8);
            for(int i = 0; i < mat.rows; ++i) {
                memcpy(image.scanLine(i), mat.ptr(i), mat.cols);
            }
            break;
        }
        case 3: {
            // Image RGB
            image = QImage(mat.cols, mat.rows, QImage::Format_RGB888);
            for(int i = 0; i < mat.rows; ++i) {
                const uchar* ptr = mat.ptr(i);
                uchar* line = image.scanLine(i);
                for(int j = 0; j < mat.cols; ++j) {
                    line[j*3 + 2] = ptr[j*3];     // B
                    line[j*3 + 1] = ptr[j*3 + 1]; // G
                    line[j*3] = ptr[j*3 + 2];     // R
                }
            }
            break;
        }
        default:
            return QImage();
        }

        return image;
    }
    catch (const std::bad_alloc&) {
        qDebug() << "Erreur d'allocation mémoire dans matToQImage";
        return QImage();
    }
}

void ProcessingWindow::displayImage(const cv::Mat& mat, QGraphicsView* view, QGraphicsScene** scene)
{
    if (!view || mat.empty())
        return;

    try {
        // Supprimer l'ancienne scène si elle existe
        if (*scene) {
            delete *scene;
            *scene = nullptr;
        }

        // Créer une nouvelle scène
        *scene = new QGraphicsScene(this);

        // Convertir et ajouter l'image
        QImage qImage = matToQImage(mat);
        if (!qImage.isNull()) {
            (*scene)->addPixmap(QPixmap::fromImage(qImage));
            view->setScene(*scene);
            view->fitInView((*scene)->sceneRect(), Qt::KeepAspectRatio);
        }
    }
    catch (const std::bad_alloc&) {
        QMessageBox::critical(this, "Erreur", "Mémoire insuffisante pour afficher l'image");
    }
}


void ProcessingWindow::on_segRGBButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée");
        return;
    }

    try {
        /*
        // Vérifier la taille de l'image
        const size_t MAX_SIZE = 1920 * 1080 * 3; // Limite pour une image HD
        if (originalImage.total() * originalImage.elemSize() > MAX_SIZE) {
            QMessageBox::warning(this, "Erreur", "Image trop grande");
            return;
        }
        */
        // Récupérer les valeurs des seuils
        uchar seuilBasR = ui->SeuilMinRVal->value();
        uchar seuilHautR = ui->SeuilMaxRVal->value();
        uchar seuilBasG = ui->SeuilMinVVal->value();
        uchar seuilHautG = ui->SeuilMaxVVal->value();
        uchar seuilBasB = ui->SeuilMinBVal->value();
        uchar seuilHautB = ui->SeuilMaxBVal->value();

        // Créer une copie de l'image pour le traitement
        cv::Mat imageTraitement = originalImage.clone();

        // Traiter l'image
        Image image(imageTraitement);
        cv::Mat resultat = image.segmentationCouleurOuNG(imageTraitement,
                                                         seuilBasR, seuilHautR,
                                                         seuilBasG, seuilHautG,
                                                         seuilBasB, seuilHautB);

        // Vérifier le résultat
        if (resultat.empty()) {
            throw std::runtime_error("Échec de la segmentation");
        }

        // Afficher le résultat
        displayImage(resultat, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::bad_alloc&) {
        QMessageBox::critical(this, "Erreur", "Mémoire insuffisante pour le traitement");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", QString("Erreur: %1").arg(e.what()));
    }
}

void ProcessingWindow::on_segmentHSV_clicked()
{

    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée");
        return;
    }

    try {
        // Récupérer les valeurs des seuils et de la taille du kernel
        int seuilBas = ui->SeuilMinHSV->value();
        int seuilHaut = ui->SeuilMaxHSV->value();
        int tailleKernel = ui->tailleNoyau->value();

        // Créer une copie de l'image pour le traitement
        cv::Mat imageTraitement = originalImage.clone();

        // Traiter l'image
        Image image(imageTraitement);
        cv::Mat hue = image.afficherTeinte(imageTraitement);

        // Créer une copie de hue pour la segmentation
        cv::Mat hueSegmentation = hue.clone();

        cv::Mat imageSegmentee = image.segmenterParTeinte(imageTraitement, hueSegmentation, seuilBas, seuilHaut, tailleKernel);

        // Afficher le résultat
        displayImage(imageSegmentee, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::bad_alloc&) {
        QMessageBox::critical(this, "Erreur", "Mémoire insuffisante pour le traitement");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", QString("Erreur: %1").arg(e.what()));
    }
}

void ProcessingWindow::on_teinteButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée");
        return;
    }

    try {
        // Créer une copie de l'image pour le traitement
        cv::Mat imageTraitement = originalImage.clone();

        // Traiter l'image
        Image image(imageTraitement);
        cv::Mat hue = image.afficherTeinte(imageTraitement);

        // Afficher le résultat
        displayImage(hue, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::bad_alloc&) {
        QMessageBox::critical(this, "Erreur", "Mémoire insuffisante pour le traitement");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", QString("Erreur: %1").arg(e.what()));
    }
}


void ProcessingWindow::on_convButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée.");
        return;
    }

    try {
        // Récupérer le type de filtre sélectionné par l'utilisateur
        QString selectedFilter = ui->convComboBox->currentText(); // Exemple : QComboBox avec les types de filtre

        // Convertir le type de filtre en `FilterType`
        FilterType filterType;
        if (selectedFilter == "MEAN") {
            filterType = FilterType::MEAN;
        } else if (selectedFilter == "SOBEL_X") {
            filterType = FilterType::SOBEL_X;
        } else if (selectedFilter == "SOBEL_Y") {
            filterType = FilterType::SOBEL_Y;
        } else if (selectedFilter == "LAPLACIAN") {
            filterType = FilterType::LAPLACIAN;
        } else if (selectedFilter == "GAUSSIAN") {
            filterType = FilterType::GAUSSIAN;
        } else {
            QMessageBox::warning(this, "Erreur", "Filtre inconnu sélectionné.");
            return;
        }

        // Créer une instance de la classe Image
        cv::Mat imageTraitement = originalImage.clone();
        Image image(imageTraitement);

        // Appliquer la convolution
        cv::Mat resultat = image.convolution2D(imageTraitement, filterType);

        // Normaliser les valeurs si nécessaire pour les afficher
        cv::normalize(resultat, resultat, 0, 255, cv::NORM_MINMAX, CV_8UC1);

        // Afficher l'image traitée
        displayImage(resultat, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
}



void ProcessingWindow::on_contourButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée.");
        return;
    }

    try {
        // Récupérer l'option sélectionnée dans le combo box
        QString selectedOption = ui->contourComboBox->currentText();

        // Déterminer si l'on utilise le gradient ou le laplacien
        bool useGradient = (selectedOption == "GRADIENT");

        // Créer une instance de la classe Image
        cv::Mat imageTraitement = originalImage.clone();
        Image image(imageTraitement);

        // Appliquer la détection de contours
        image.detectionContours(useGradient);

        // Récupérer les résultats depuis l'objet Image
        cv::Mat resultat = image.getData(); // Assurez-vous que `Image` dispose d'un getter pour les données d'image

        // Afficher l'image traitée
        displayImage(resultat, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
}


void ProcessingWindow::on_RehaussButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée.");
        return;
    }

    try {
        // Déterminer l'option de réhaussement des contours (Gradient ou Laplacien)
        QString selectedOption = ui->contourComboBox->currentText();
        bool gradOrLap = (selectedOption == "GRADIENT"); // true pour Gradient, false pour Laplacien

        // Créer une instance de la classe Image
        Image image;

        // Appliquer le réhaussement des contours
        cv::Mat resultat = image.rehaussementContour(originalImage, gradOrLap);

        // Vérifier si le résultat est vide
        if (resultat.empty()) {
            QMessageBox::critical(this, "Erreur", "Le réhaussement des contours a échoué.");
            return;
        }

        // Afficher l'image réhaussée dans la vue graphique
        displayImage(resultat, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
}


void ProcessingWindow::on_HistoButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée.");
        return;
    }

    try {
        QString selectedOption = ui->histogramComboBox->currentText();
        Image image;
        bool isGray = originalImage.channels() == 1;

        if (isGray && (selectedOption == "Bleu" || selectedOption == "Vert" || selectedOption == "Rouge")) {
            QMessageBox::warning(this, "Erreur", "L'image est en niveaux de gris. Vous ne pouvez pas afficher les histogrammes des canaux de couleur.");
            return;
        }

        Mat histogramImage;

        if (isGray) {
            histogramImage = image.calculateAndDisplayHistogram(originalImage, "Niv_Gris");
        } else {
            std::vector<Mat> channels;
            cv::Mat bgrImage;

            // Convertir explicitement en BGR si nécessaire
            if (originalImage.channels() == 3) {
                bgrImage = originalImage.clone();
            } else {
                cv::cvtColor(originalImage, bgrImage, cv::COLOR_RGBA2BGR);
            }

            cv::split(bgrImage, channels);

            if (selectedOption == "Niv_Gris") {
                histogramImage = image.calculateAndDisplayHistogram(channels);
            } else {
                int channelIndex = (selectedOption == "Bleu") ? 0 :
                                       (selectedOption == "Vert") ? 1 : 2;
                histogramImage = image.calculateAndDisplayHistogram(channels[channelIndex], selectedOption.toStdString());
            }
        }

        if (histogramImage.empty()) {
            QMessageBox::critical(this, "Erreur", "L'histogramme n'a pas pu être calculé.");
            return;
        }

        displayImage(histogramImage, ui->ImageResultatgraphicsView, &sceneResult);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
}
