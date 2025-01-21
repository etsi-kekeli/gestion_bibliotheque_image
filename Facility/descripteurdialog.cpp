#include "descripteurdialog.h"
#include "ui_descripteurdialog.h"
#include <QFileDialog>


QString accesToString(Acces a){
    if (a == Acces::LIMITE) return "Limité";
    return "Ouvert";
}

Acces stringToAcces(QString s){
    if (s == "Limité") return Acces::LIMITE;
    return Acces::OUVERT;
}

DescripteurDialog::DescripteurDialog(Descripteur *d, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DescripteurDialog)
{
    ui->setupUi(this);
    this->d = d;

    ui->edCout->setValidator(new QDoubleValidator(0.0, 100.0, 2, this));

    ui->accesBox->setCurrentText(accesToString(d->getAcces()));
    ui->edAuteur->setText(QString::fromStdString(d->getAuteur()));
    ui->edCout->setText(QString::number(d->getCout()));
    ui->edTitre->setText(QString::fromStdString(d->getTitre()));
    ui->edSource->setText(QString::fromStdString(d->getSource()));
}

DescripteurDialog::~DescripteurDialog()
{
    delete ui;
}

void DescripteurDialog::updateDescripteur(){
    d->setTitre(ui->edTitre->text().toStdString());
    d->setCout(ui->edCout->text().toDouble());
    d->setAuteur(ui->edAuteur->text().toStdString());
    d->setAcces(stringToAcces(ui->accesBox->currentText()));
    d->setSource(ui->edSource->text().toStdString());
}

void DescripteurDialog::on_buttonBox_accepted()
{
    updateDescripteur();
    this->close();
}


void DescripteurDialog::on_btnParcourir_clicked()
{
    QString chemin = QFileDialog::getOpenFileName(this,
                                                    "Choisir une image",
                                                    QDir::homePath(),
                                                    "All Files (*.*);;Images (*.png *.jpg *.jpeg)");

    if (chemin.isEmpty()) return;

    ui->edSource->setText(chemin);
}
