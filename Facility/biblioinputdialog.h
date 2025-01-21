#ifndef BIBLIOINPUTDIALOG_H
#define BIBLIOINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "../routines/Bibliotheque.h"

class BiblioInputDialog : public QDialog
{
public:
    BiblioInputDialog(Bibliotheque* b, QWidget* parent = nullptr);
    std::string getNom();
private slots:
    void setBiblioName();
private:
    QLineEdit *inputField;
    Bibliotheque* biblio;
};

#endif // BIBLIOINPUTDIALOG_H
