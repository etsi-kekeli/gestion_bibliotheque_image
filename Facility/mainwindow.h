#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gallerie.h>
#include "../routines/Utilisateur.h"
#include "../routines/Bibliotheque.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructeur avec l'objet Utilisateur et le parent
    explicit MainWindow(Utilisateur& utilisateur, QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btnCreer_clicked();

    void on_btnCharger_clicked();

    void on_btnSauvegarder_clicked();

    void on_btnImage_clicked();

    void on_btnSupprimer_clicked();

    void on_btnTrier_clicked();

    void on_btnPrix_clicked();

    void on_btnFiltrer_clicked();

private:
    void updateStats();
    Ui::MainWindow *ui;
    Utilisateur m_utilisateur;  // Déclaration de l'objet Utilisateur
    Gallerie* galerie;
    Bibliotheque* biblio = nullptr;
};

#endif // MAINWINDOW_H

/*
private slots:
    void connectionWidget();
    void mainWidget();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
*/
