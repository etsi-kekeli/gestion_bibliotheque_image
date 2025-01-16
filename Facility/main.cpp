#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
   // w.show();

    // Créer la fenêtre de login
    LoginWindow loginWindow;
    loginWindow.exec(); // Affiche la fenêtre de login et attend que l'utilisateur se connecte

    // Une fois la connexion réussie, ouvrir MainWindow (elle s'ouvre depuis LoginWindow)
    return a.exec();
}
