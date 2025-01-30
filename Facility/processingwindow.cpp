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
#include <QPixmap>
#include <QTimer>

ProcessingWindow::ProcessingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessingWindow)
    , sceneOriginal(nullptr)
    , sceneResult(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Traitement des images");
    this->setWindowIcon(QIcon(":/FacilityLogo/FacilityLogo/Logo.png"));
}

ProcessingWindow::ProcessingWindow(const QString& imagePath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessingWindow)
    , sceneOriginal(nullptr)
    , sceneResult(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Traitement des images");
    this->setWindowIcon(QIcon(":/FacilityLogo/FacilityLogo/Logo.png"));

    loadImage(imagePath);
 // Ajout d'un délai pour forcer l'ajustement après l'affichage complet de la fenêtre
    QTimer::singleShot(100, this, [this]() {
        if (ui->ImageOriginalegraphicsView->scene()) {
            ui->ImageOriginalegraphicsView->fitInView(ui->ImageOriginalegraphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
        }
         });
}


ProcessingWindow::~ProcessingWindow()
{
    delete sceneOriginal;
    delete sceneResult;
    delete ui;
}

// surcharger l'événement resizeEvent Pour que l'image s'ajuste automatiquement lorsque la fenêtre est redimensionnée
void ProcessingWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Ajuster l'image originale si la scène existe
    if (ui->ImageOriginalegraphicsView->scene()) {
        ui->ImageOriginalegraphicsView->fitInView(ui->ImageOriginalegraphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }

    // Ajuster l'image traitée si la scène existe
    if (ui->ImageResultatgraphicsView->scene()) {
        ui->ImageResultatgraphicsView->fitInView(ui->ImageResultatgraphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

// méthode pour charger l'image a partir du constructeur qui prend image path comme attribut
void ProcessingWindow::loadImage(const QString& imagePath)
{
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
    cv::Mat cvImage = cv::imread(imagePath.toStdString(), cv::IMREAD_UNCHANGED);
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
            cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2GRAY);
        }
    }

    Image myImage(cvImage);

    myImage.setTitreImage(imagePath.toStdString());

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

    // Vider l'affichage de l'image résultante
    if (ui->ImageResultatgraphicsView->scene() != nullptr) {
        delete ui->ImageResultatgraphicsView->scene();
    }
    ui->ImageResultatgraphicsView->setScene(new QGraphicsScene(this));
    ui->ImageResultatgraphicsView->update();

    ProcessingWindow::on_RestValRGBSeuilButton_clicked();
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
    // Vérifier si une image est affichée dans ImageResultatgraphicsView
    if (!sceneResult || sceneResult->items().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image affichée dans ImageResultatgraphicsView à sauvegarder.");
        return;
    }

    // Récupérer l'image de la scène
    QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(sceneResult->items().at(0));
    if (!pixmapItem) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération de l'image.");
        return;
    }

    QPixmap pixmap = pixmapItem->pixmap();

    // Convertir QPixmap en cv::Mat
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR); // Convertir de RGB (Qt) à BGR (OpenCV)

    // Ouvrir une boîte de dialogue pour sélectionner le chemin et le nom du fichier
    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer l'image",
        QDir::homePath(), // Répertoire par défaut
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;PGM (*.pgm);;TIFF (*.tif);;Tous les fichiers (*)",
        &selectedFilter);

    // Vérifier si l'utilisateur a annulé la boîte de dialogue
    if (filePath.isEmpty()) {
        return;
    }

    // Identifier l'extension à partir du filtre sélectionné
    QString extension;
    if (selectedFilter.contains("*.png")) {
        extension = "png";
    } else if (selectedFilter.contains("*.jpg") || selectedFilter.contains("*.jpeg")) {
        extension = "jpg";
    } else if (selectedFilter.contains("*.bmp")) {
        extension = "bmp";
    } else if (selectedFilter.contains("*.pgm")) {
        extension = "pgm";
    } else if (selectedFilter.contains("*.tif")) {
        extension = "tif";
    }

    // Ajouter l'extension au chemin si elle n'est pas déjà présente
    QFileInfo fileInfo(filePath);
    if (fileInfo.suffix().isEmpty() || fileInfo.suffix().toLower() != extension) {
        filePath += "." + extension;
    }

    // Convertir le chemin en string pour OpenCV
    std::string outputPath = filePath.toStdString();

    // Sauvegarder l'image résultante avec OpenCV
    try {
        bool success = cv::imwrite(outputPath, mat);
        if (!success) {
            throw std::runtime_error("Échec de l'enregistrement de l'image.");
        }

        QMessageBox::information(this, "Succès", "Image enregistrée avec succès !");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
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
    if (!view || mat.empty()) {
        qDebug() << "View invalide ou matrice vide";
        return;
    }

    try {
        if (*scene) {
            delete *scene;
            *scene = nullptr;
        }

        *scene = new QGraphicsScene(this);
        QImage qImage = matToQImage(mat);

        if (!qImage.isNull()) {
            QPixmap pixmap = QPixmap::fromImage(qImage);
            qDebug() << "Dimensions du pixmap:" << pixmap.width() << "x" << pixmap.height();

            (*scene)->clear();
            (*scene)->addPixmap(pixmap);
            view->setScene(*scene);
            view->fitInView((*scene)->sceneRect(), Qt::KeepAspectRatio);

            // Force le rafraîchissement
            view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            view->viewport()->update();
            view->update();

            qDebug() << "Scene rect:" << (*scene)->sceneRect();
        } else {
            qDebug() << "Conversion en QImage échouée";
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Erreur dans displayImage:" << e.what();
    }
}

void ProcessingWindow::on_segRGBButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée");
        return;
    }
    try {
        uchar seuilBasR = ui->SeuilMinRVal->value();
        uchar seuilHautR = ui->SeuilMaxRVal->value();
        uchar seuilBasG = ui->SeuilMinVVal->value();
        uchar seuilHautG = ui->SeuilMaxVVal->value();
        uchar seuilBasB = ui->SeuilMinBVal->value();
        uchar seuilHautB = ui->SeuilMaxBVal->value();

        qDebug() << "Avant segmentation:";
        qDebug() << "- Type image:" << originalImage.type();
        qDebug() << "- Canaux:" << originalImage.channels();
        qDebug() << "- Dimensions:" << originalImage.size().width << "x" << originalImage.size().height;
        qDebug() << "- Seuils R:" << (int)seuilBasR << "-" << (int)seuilHautR;
        qDebug() << "- Seuils G:" << (int)seuilBasG << "-" << (int)seuilHautG;
        qDebug() << "- Seuils B:" << (int)seuilBasB << "-" << (int)seuilHautB;

        cv::Mat imageTraitement = originalImage.clone();
        Image image(imageTraitement);
        cv::Mat resultat = image.segmentationCouleurOuNG(imageTraitement,
                                                         seuilBasR, seuilHautR,
                                                         seuilBasG, seuilHautG,
                                                         seuilBasB, seuilHautB);

        qDebug() << "Après segmentation:";
        qDebug() << "- Type résultat:" << resultat.type();
        qDebug() << "- Canaux:" << resultat.channels();
        qDebug() << "- Dimensions:" << resultat.size().width << "x" << resultat.size().height;

        // Vérifions si l'image n'est pas complètement noire
        cv::Scalar moyenne = cv::mean(resultat);
        qDebug() << "- Moyenne des canaux:" << moyenne[0] << moyenne[1] << moyenne[2];

        if (resultat.empty()) {
            throw std::runtime_error("Échec de la segmentation");
        }

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

void ProcessingWindow::on_HoughButton_clicked()
{
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image chargée.");
        return;
    }

    try {
        // Initialisation de l'objet Image avec l'image en mémoire
        Image myImage(originalImage);

        // Paramètres pour la transformée de Hough
        int nRho = 400;         // Nombre de divisions pour rho
        int nTheta = 360;       // Nombre de divisions pour theta
        int voisinage = 15;     // Taille du voisinage pour la détection de maxima
        int seuil = 0.1 * std::sqrt(std::pow(myImage.getData().rows, 2) + std::pow(myImage.getData().cols, 2));        // Seuil pour les votes

        // Calcul et affichage de la table d'accumulation normalisée
        cv::Mat houghTable = myImage.getTableDAccumulationNormalisee(nRho, nTheta);
        if (houghTable.empty()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors du calcul de la table d'accumulation.");
            return;
        }
        displayImage(houghTable, ui->ImageResultatgraphicsView, &sceneResult);

        // Dessiner les lignes détectées sur l'image originale
        cv::Mat imageAvecLignes = myImage.dessineLigneHough(myImage.getData().rows, nTheta/2, voisinage, seuil, cv::Scalar(0, 0, 255), 2);
        if (imageAvecLignes.empty()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors du dessin des lignes de Hough.");
            return;
        }
        displayImage(imageAvecLignes, ui->ImageOriginalegraphicsView, &sceneOriginal);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString("Une erreur s'est produite : %1").arg(e.what()));
    }
}


cv::Mat ProcessingWindow::QPixmapToCvMat(const QPixmap &pixmap) {
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    cv::Mat mat = cv::Mat(img.height(), img.width(), CV_8UC4, (void*)img.bits(), img.bytesPerLine());
    // Conversion en BGR si nécessaire
    cv::Mat matBGR;
    cv::cvtColor(mat, matBGR, cv::COLOR_BGRA2BGR);
    return matBGR.clone();
}


void ProcessingWindow::on_actionSwitch_triggered()
{
    if (!ui->ImageResultatgraphicsView || !ui->ImageResultatgraphicsView->scene()) {
        qDebug() << "Scene ou view invalide";
        return;
    }

    QGraphicsScene* scene = ui->ImageResultatgraphicsView->scene();
    if (scene->items().isEmpty()) {
        qDebug() << "Scene vide";
        return;
    }

    QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (!pixmapItem) {
        qDebug() << "Pas de pixmap item";
        return;
    }

    QPixmap pixmap = pixmapItem->pixmap();
    if (pixmap.isNull()) {
        qDebug() << "Pixmap invalide";
        return;
    }

    cv::Mat newOriginalImage = QPixmapToCvMat(pixmap);
    if (newOriginalImage.empty()) {
        qDebug() << "Conversion cv::Mat échouée";
        return;
    }

    // Ajout de debug pour vérifier les dimensions
    qDebug() << "Dimensions de newOriginalImage:" << newOriginalImage.cols << "x" << newOriginalImage.rows;

    originalImage = newOriginalImage.clone();

    // Force le rafraîchissement complet
    if (sceneOriginal) {
        delete sceneOriginal;
        sceneOriginal = nullptr;
    }

    displayImage(originalImage, ui->ImageOriginalegraphicsView, &sceneOriginal);

    // Force la mise à jour
    ui->ImageOriginalegraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->ImageOriginalegraphicsView->viewport()->update();
    ui->ImageOriginalegraphicsView->update();
}
