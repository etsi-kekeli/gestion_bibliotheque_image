#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "../routines/Utilisateur.h"
#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_LoginbuttonBox_accepted()
{
    QString codeSaisi = ui->codeLineEdit->text();

    try {
        // Créer un objet Utilisateur avec le code saisi
        Utilisateur utilisateur(codeSaisi.toStdString());

        // Vérifier le niveau d'accès
        if (utilisateur.getNiveau() == Utilisateur::Niveau::NIVEAU1 || utilisateur.getNiveau() == Utilisateur::Niveau::NIVEAU2) {
            // Ouvrir MainWindow
            this->hide(); // Masquer la fenêtre de login
            MainWindow *mainWindow = new MainWindow(utilisateur, this);
            mainWindow->show();
        } else {
            // Si l'utilisateur n'a pas les droits, afficher un message d'erreur
            QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les droits nécessaires.");
            ui->codeLineEdit->clear();  // Effacer le champ pour un nouvel essai
            ui->codeLineEdit->setFocus(); // Redonner le focus sur le champ de saisie
            // Garder la fenêtre ouverte pour permettre un autre essai
            this->show(); // Rouvrir la fenêtre de login
        }
    } catch (const std::exception &e) {
        // Si le code est invalide, afficher un message d'erreur
        QMessageBox::critical(this, "Erreur", e.what());
        ui->codeLineEdit->clear();  // Effacer le champ pour un nouvel essai
        ui->codeLineEdit->setFocus(); // Redonner le focus sur le champ de saisie
        // Garder la fenêtre ouverte pour permettre un autre essai
        this->show(); // Rouvrir la fenêtre de login
    }
}



void LoginWindow::on_LoginbuttonBox_rejected()
{
    // Lorsque l'utilisateur annule la connexion, on ferme la fenêtre de login
    this->close(); // Fermer la fenêtre de login
}

