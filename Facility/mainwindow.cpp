#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Utilisateur& utilisateur, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_utilisateur(utilisateur)  // Initialisation de m_utilisateur
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::connectionWidget() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::mainWidget() {
    std::string code = ui->btnConnect->text().toStdString();
}
*/
