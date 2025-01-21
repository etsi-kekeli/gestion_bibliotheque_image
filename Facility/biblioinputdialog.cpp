#include "biblioinputdialog.h"

BiblioInputDialog::BiblioInputDialog(Bibliotheque* b, QWidget *parent) : biblio(b), QDialog(parent) {

    QLabel *label = new QLabel("Entrer le nom de la bibliothèque", this);
    inputField = new QLineEdit(this);
    QPushButton *btnOk = new QPushButton("OK", this);

    connect(btnOk, &QPushButton::clicked, this, &BiblioInputDialog::setBiblioName);

    // Layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(inputField);
    layout->addWidget(btnOk);

    setLayout(layout);
}

std::string BiblioInputDialog::getNom() {
    return inputField->text().toStdString();
}

void BiblioInputDialog::setBiblioName() {
    this->biblio->setNom(getNom());
    this->close();
}
