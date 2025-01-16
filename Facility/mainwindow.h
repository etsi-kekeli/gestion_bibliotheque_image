#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../routines/Utilisateur.h"

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


private:
    Ui::MainWindow *ui;
    Utilisateur m_utilisateur;  // Déclaration de l'objet Utilisateur
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
