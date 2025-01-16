#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "ui_loginwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    // Slot pour valider la connexion
     void on_LoginbuttonBox_accepted();
    // Slot pour annuler la connexion
    void on_LoginbuttonBox_rejected();



private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
