#include "afficheurdescripteur.h"

AfficheurDescripteur::AfficheurDescripteur(Descripteur* d, QWidget *parent)
    : d(new Descripteur()), QWidget{parent}, ui(new Ui::AfficheurDescripteur)
{
    ui->setupUi(this);
    this->d = d;
    this->ui->lblID->setText("ID : " + QString::number(d->getIdDescripteur()));
    this->ui->lblCout->setText("Cout + " + QString::number(d->getCout()));
    this->ui->lblAuteur->setText("Auteur : " + QString::fromStdString(d->getAuteur()));
    this->ui->lblTitre->setText("Titre : " + QString::fromStdString(d->getTitre()));

    pix = QPixmap(QString::fromStdString(d->getSource()));
    if (pix.isNull()){
        pix = QPixmap(":/FacilityLogo/FacilityLogo/Logo.png");
    }
    int h = pix.height();
    int w = pix.width();
    ui->lblImage->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));

    show();
}

AfficheurDescripteur::~AfficheurDescripteur(){
    delete ui;
}
