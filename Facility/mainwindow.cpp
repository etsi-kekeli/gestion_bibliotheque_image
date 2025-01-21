#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "biblioinputdialog.h"
#include "descripteurdialog.h"

MainWindow::MainWindow(Utilisateur& utilisateur, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_utilisateur(utilisateur)  // Initialisation de m_utilisateur
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/FacilityLogo/FacilityLogo/Logo.png"));
    this->setWindowTitle("Main");
    galerie = new Gallerie(this);
    ui->contGalerie->setLayout(new QVBoxLayout);
    ui->contGalerie->layout()->addWidget(galerie);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCreer_clicked()
{
    if (!biblio) biblio = new Bibliotheque();

        BiblioInputDialog* dialogue = new BiblioInputDialog(biblio, this);
        dialogue->exec();

    // std::cout<<"Done in slot"<<std::endl;
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
            this->galerie->raffrachir(biblio->getDescripteurs());
            update();
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
    }
    else {
        delete d;
    }

    delete dialogue;

}

