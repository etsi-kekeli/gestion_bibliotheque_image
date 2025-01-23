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
    explicit AfficheurDescripteur(Descripteur* d, std::function<void(const std::string&)> f, std::function<void()> g, QWidget *parent = nullptr);
    ~AfficheurDescripteur();

private:
    Ui::AfficheurDescripteur *ui;
    Descripteur *d;
    QPixmap pix;
    std::function<void(const std::string&)> supprimerDescripteur;
    std::function<void()> mettreAJourUi;

signals:
private slots:
    void on_btnSupprimer_clicked();
    void on_btnModifier_clicked();
};

#endif // AFFICHEURDESCRIPTEUR_H
