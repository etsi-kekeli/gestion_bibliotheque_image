#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "biblioinputdialog.h"
#include "descripteurdialog.h"
#include <QIntValidator>
#include <QDoubleValidator>

MainWindow::MainWindow(Utilisateur& utilisateur, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_utilisateur(utilisateur)  // Initialisation de m_utilisateur
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/FacilityLogo/FacilityLogo/Logo.png"));
    this->setWindowTitle("Main");
    galerie = new Gallerie(biblio, m_utilisateur.getNiveau(), [this]() { this->updateStats(); }, this);
    ui->contGalerie->setLayout(new QVBoxLayout);
    ui->contGalerie->layout()->addWidget(galerie);

    ui->statusbar->addPermanentWidget(ui->btnUtilisateur);
    ui->statusbar->addPermanentWidget(ui->lblBib);

    ui->lblBib->setText("Bibliothèque : ");

    ui->btnUtilisateur->setText("Déconnecter " + QString::fromStdString(m_utilisateur.getCode()));


    ui->edID->setValidator(new QIntValidator(0, 1000, ui->edID));
    ui->edCoutMax->setValidator(new QDoubleValidator(0.0, 1e5, 2, this));
    ui->edCoutMin->setValidator(new QDoubleValidator(0.0, 1e5, 2, this));

    if (m_utilisateur.getNiveau() == Utilisateur::Niveau::NIVEAU1) updateStats();
    else if (m_utilisateur.getNiveau() == Utilisateur::Niveau::NIVEAU2) appliquerNiveau2();
    else appliquerNiveau3();
}

void MainWindow::appliquerNiveau2(){
    ui->btnCreer->setDisabled(true);
    ui->btnFiltrer->setDisabled(true);
    ui->btnSupprimer->setDisabled(true);
    ui->btnSauvegarder->setDisabled(true);
    ui->btnImage->setDisabled(true);

    ui->btnTrier->setDisabled(true);
    ui->btnPrix->setDisabled(true);
}

void MainWindow::appliquerNiveau3(){
    appliquerNiveau2();
    ui->btnCharger->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enleverDescripteur(){
    updateStats();
}

void MainWindow::on_btnCreer_clicked()
{
    Bibliotheque* b = new Bibliotheque();

    BiblioInputDialog* dialogue = new BiblioInputDialog(b, this);
    if (dialogue->exec() != QDialog::Accepted ) return;

    if (biblio) delete biblio;

    biblio = b;
    galerie->setBib(biblio);
    // std::cout<<"Done in slot"<<std::endl;
    updateStats();

    ui->lblBib->setText("Bibliothèque : " + QString::fromStdString(biblio->getNom()));
}

void MainWindow::on_btnCharger_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choisissez le fichier de sauvegarde ", QDir::homePath(), "*.fty;;Tous les fichiers (*.*)");

    if (!path.isEmpty()){
        Bibliotheque* new_biblio = new Bibliotheque();

        try {
            new_biblio->chargerBibliotheque(path.toStdString());
            if (biblio) delete biblio;
            biblio = new_biblio;
            galerie->setBib(biblio);
            galerie->vider();

            ui->lblBib->setText("Bibliothèque : " + QString::fromStdString(biblio->getNom()));


            if (m_utilisateur.getNiveau() == Utilisateur::Niveau::NIVEAU1){
                this->galerie->raffrachir(biblio->getDescripteurs());
                update();
                updateStats();
            }
            else {
                galerie->raffrachir(biblio->getDescripteursLibres());
            }

        } catch (Exception q) {
            delete new_biblio;
            QMessageBox::information(this, "Échec", "Échec de chargement du fichier");
        }
    }
}


void MainWindow::on_btnSauvegarder_clicked()
{
    QString chemin = QFileDialog::getSaveFileName(this,
                                                    "Sauvegarder la bibliothèque",
                                                    QDir::homePath(),
                                                    "(*.fty)");

    if (!chemin.isEmpty() && biblio) {
        if (!chemin.endsWith(".fty")) chemin = chemin + ".fty";

        bool ret = biblio->sauvegarderBibliotheque(chemin.toStdString());

        if (ret) QMessageBox::information(this, "Succes", "La bibliothèque a été bien sauvegardée");

    }
}


void MainWindow::on_btnImage_clicked()
{
    Descripteur *d = new Descripteur;
    if (!biblio){
        QMessageBox::information(this, "Échec", "Aucune bibliothèque n'est active");
        return;
    }
    if (biblio->getDescripteurs()->empty()){
        d->setIdDescripteur(1);
    }
    else{
        int id = biblio->getDescripteurs()->back()->getIdDescripteur() + 1;
        d->setIdDescripteur(id);
    }

    DescripteurDialog * dialogue = new DescripteurDialog(d, this);
    if (dialogue->exec() == QDialog::Accepted){
        biblio->ajouterDescripteur(d);
        galerie->raffrachir(biblio->getDescripteurs());
        update();
        updateStats();
    }
    else {
        delete d;
    }

    delete dialogue;

}


void MainWindow::on_btnSupprimer_clicked()
{
    if (biblio &&
        QMessageBox::question(this,
                                "Confirmer votre action",
                                "Voulez supprimer la bibliothèque courante ? Si elle est enrégistrée sur un fichier, la sauvegarde persistera sur le disque",
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        delete biblio;
        biblio = nullptr;
        this->galerie->vider();
        updateStats();

        ui->lblBib->setText("Bibliothèque : ");
    }
}


void MainWindow::on_btnTrier_clicked()
{
    if (!biblio) return;

    vector<Descripteur*>* v = biblio->trierDescripteurs();
    galerie->vider();
    galerie->raffrachir(v);
}


void MainWindow::on_btnPrix_clicked()
{

    double prix = 0;
    if (ui->edID->text().isEmpty() || !biblio) return;

    int ID = ui->edID->text().toInt();

    prix = biblio->chercherCoutImage(ID);

    QString message = (prix < 0) ? "Il n'y a pas d'image avec ce ID dans la bibliothèque" : "Limage d'ID : " + QString::number(ID) + " vaut " + QString::number(prix);
    QMessageBox::information(this, "Prix", message);
}


void MainWindow::on_btnFiltrer_clicked()
{
    if (!biblio) return;
    QString min_str = ui->edCoutMin->text();
    QString max_str = ui->edCoutMax->text();

    double min_prix = min_str.isEmpty() ? 0 : min_str.toDouble();
    double max_prix = max_str.isEmpty() ?  numeric_limits<float>::max() : max_str.toDouble();

    vector<Descripteur*>* d = biblio->filter(min_prix, max_prix);

    galerie->vider();
    galerie->raffrachir(d);
}

void MainWindow::updateStats(){
    double min = 0,  max = 0, moy = 0;
    int libres = 0;

    if (biblio) {
        min = biblio->calculerCoutMin();
        max = biblio->calculerCoutMax();
        moy = biblio->calculerCoutMoyen();
        libres = biblio->nombreDImagesLibres();
    }
    ui->statMin->setText("Coût min : " + QString::number(min));
    ui->statMax->setText("Coût max : " + QString::number(max));
    ui->statMoy->setText("Coût moyen : " + QString::number(moy, 'f', 2));
    ui->statLibre->setText("Images libres : " + QString::number(libres));

}


void MainWindow::on_btnUtilisateur_clicked()
{
    LoginWindow * log = new LoginWindow(nullptr);
    log->show();

    this->close();
}

