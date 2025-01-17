#include "mainwindow.h"
#include "loginwindow.h"
#include "processingwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
   // w.show();

    // Créer la fenêtre de login
    LoginWindow loginWindow;
    loginWindow.exec(); // Affiche la fenêtre de login et attend que l'utilisateur se connecte
    /*
    // Fenetre de traitement des images (appelée dans le main seulement pour tester)
    ProcessingWindow prosWin;
    prosWin.show();
    // Une fois la connexion réussie, ouvrir MainWindow (elle s'ouvre depuis LoginWindow)
    */
    return a.exec();
}
