#ifndef AFFICHEURDESCRIPTEUR_H
#define AFFICHEURDESCRIPTEUR_H

#include <QWidget>
#include "ui_afficheurdescripteur.h"

#include "../routines/Descripteur.h"

namespace Ui {
class AfficheurDescripteur;
}

class AfficheurDescripteur : public QWidget
{
    Q_OBJECT
public:
    explicit AfficheurDescripteur(Descripteur* d, QWidget *parent = nullptr);
    ~AfficheurDescripteur();

private:
    Ui::AfficheurDescripteur *ui;
    Descripteur *d;
    QPixmap pix;

signals:
};

#endif // AFFICHEURDESCRIPTEUR_H
