#include "afficheurdescripteur.h"
#include <QMessageBox>
#include "gallerie.h"
#include "descripteurdialog.h"

AfficheurDescripteur::AfficheurDescripteur(Descripteur* d,  std::function<void(const std::string&)> f, std::function<void()> g, QWidget *parent)
    : d(new Descripteur()), supprimerDescripteur(f), mettreAJourUi(g), QWidget{parent}, ui(new Ui::AfficheurDescripteur)
{
    ui->setupUi(this);
    this->d = d;
    this->ui->lblID->setText("ID : " + QString::number(d->getIdDescripteur()));
    this->ui->lblCout->setText("Cout : " + QString::number(d->getCout()));
    this->ui->lblAuteur->setText("Auteur : " + QString::fromStdString(d->getAuteur()));
    this->ui->lblTitre->setText("Titre : " + QString::fromStdString(d->getTitre()));

    pix = QPixmap(QString::fromStdString(d->getSource()));
    if (pix.isNull()) {
        pix = QPixmap(":/FacilityLogo/FacilityLogo/Logo.png");
    }
    int h = pix.height();
    int w = pix.width();
    ui->lblImage->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    ui->lblImage->setMinimumSize(100, 100);

    setMinimumSize(220, 280);
    show();
}

AfficheurDescripteur::~AfficheurDescripteur(){
    delete ui;
}

void AfficheurDescripteur::on_btnSupprimer_clicked()
{
    if (QMessageBox::question(this,
                              "Confirmer votre action",
                              "Voulez supprimer cette image de la bibliothèque ? L'image ne sera pas supprimé sur le disque",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        supprimerDescripteur(d->getSource());
    }
}

void AfficheurDescripteur::on_btnModifier_clicked()
{
    DescripteurDialog* dialogue = new DescripteurDialog(d, this);
    dialogue->exec();
    mettreAJourUi();
    delete dialogue;
}

