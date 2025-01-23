#ifndef GALLERIE_H
#define GALLERIE_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include "../routines/Bibliotheque.h"
#include "afficheurdescripteur.h"

class Gallerie : public QScrollArea
{
    Q_OBJECT
public:
    explicit Gallerie(Bibliotheque* b, std::function<void()> updateStats, QWidget *parent = nullptr);
    ~Gallerie();
    void raffrachir(std::vector<Descripteur*>* descripteurs = nullptr);
    void enleverDescripteur(const string& s);
    void vider();
    void setBib(Bibliotheque* b);

private:
    // QScrollArea* scrollArea;
    QWidget* container;
    QGridLayout* layout;
    Bibliotheque* bib;
    std::function<void()> updateStats;

signals:
};

#endif // GALLERIE_H
