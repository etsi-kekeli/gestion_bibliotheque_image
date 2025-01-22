#include "gallerie.h"

Gallerie::Gallerie(QWidget *parent)
    : QScrollArea{parent}
{
    layout = new QGridLayout(this);
    container = new QWidget(this);
    // QVBoxLayout* vlayout = new QVBoxLayout(this);
    // setLayout(vlayout);

    container->setLayout(layout);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidget(container);
    setWidgetResizable(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

Gallerie::~Gallerie(){
    delete layout;
}

void Gallerie::vider(){
    if (!layout) {
        return;
    }

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Gallerie::raffrachir(std::vector<Descripteur*>* descripteurs){
    int row = 0, col = 0;

    for (Descripteur* d : *descripteurs){
        AfficheurDescripteur* afficheur = new AfficheurDescripteur(d, this);
        layout->addWidget(afficheur, row, col);

    /*
    CONNECTION À FAIRE POUR SUPPRESSION, MODIFICATION ET TRAITEMENT
    */

        if (++col >= 3){
            col = 0;
            row++;
        }
    }

    update();
    show();

}
