#include "gallerie.h"

Gallerie::Gallerie(Bibliotheque* b, std::function<void()> f, QWidget *parent)
    : bib(b), updateStats(f), QScrollArea{parent}
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

void Gallerie::setBib(Bibliotheque *b) {
    bib = b;
}
void Gallerie::enleverDescripteur(const string& s) {
    bib->enleveDescripteur(s);
    vider();
    raffrachir(bib->getDescripteurs());
    updateStats();
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
        AfficheurDescripteur* afficheur = new AfficheurDescripteur(d,
                                                                   [this](const std::string& s) { this->enleverDescripteur(s); },
                                                                   [this]() {
                                                                        this->vider();
                                                                        this->raffrachir(this->bib->getDescripteurs());
                                                                        this->updateStats();
                                                                    },
                                                                   this);
        layout->addWidget(afficheur, row, col);

        // if (afficheur->parentWidget()->parentWidget()->parentWidget() == this) cout<<"Parent"<<endl;
        // else if (!afficheur->parentWidget()) cout << "null !" <<endl;
        // else cout << "No no !" <<endl;

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
