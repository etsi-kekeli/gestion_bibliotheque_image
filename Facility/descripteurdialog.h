#ifndef DESCRIPTEURDIALOG_H
#define DESCRIPTEURDIALOG_H

#include <QDialog>
#include "../routines/Descripteur.h"

namespace Ui {
class DescripteurDialog;
}

class DescripteurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DescripteurDialog(Descripteur *d, QWidget *parent = nullptr);
    ~DescripteurDialog();
    void disableBtnParcourir();
    void enableBtnParcourir();

private slots:
    void updateDescripteur();

    void on_buttonBox_accepted();

    void on_btnParcourir_clicked();

private:
    Ui::DescripteurDialog *ui;
    Descripteur *d;
};

#endif // DESCRIPTEURDIALOG_H
