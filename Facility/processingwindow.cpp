#include "processingwindow.h"
#include "ui_processingwindow.h"

ProcessingWindow::ProcessingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessingWindow)
{
    ui->setupUi(this);
}

ProcessingWindow::~ProcessingWindow()
{
    delete ui;
}

void ProcessingWindow::on_SeuilDial_valueChanged(int value)
{
    ui->SeuilValLabel->setText(QString::number(value));
}


void ProcessingWindow::on_RestValRGBSeuilButton_clicked()
{
    ui->SeuilMinRVal->setValue(0);
    ui->SeuilMinVVal->setValue(0);
    ui->SeuilMinBVal->setValue(0);
    ui->SeuilMaxRVal->setValue(0);
    ui->SeuilMaxVVal->setValue(0);
    ui->SeuilMaxBVal->setValue(0);
}


void ProcessingWindow::on_actionOuvrir_triggered()
{

}


void ProcessingWindow::on_actionSortir_triggered()
{
    this->close();
}

