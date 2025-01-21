#include "gallerie.h"

Gallerie::Gallerie(QWidget *parent)
    : QWidget{parent}
{
    scrollArea = new QScrollArea(this);
    container = new QWidget();
    layout = new QGridLayout(container);

    container->setLayout(layout);
    scrollArea->setWidget(container);
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(scrollArea);
    setLayout(pLayout);
    update();
    show();
}

Gallerie::~Gallerie(){
    delete scrollArea;
    delete container;
    delete layout;
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
