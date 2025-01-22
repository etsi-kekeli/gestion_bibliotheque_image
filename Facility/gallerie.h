#ifndef GALLERIE_H
#define GALLERIE_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>

#include "afficheurdescripteur.h"

class Gallerie : public QScrollArea
{
    Q_OBJECT
public:
    explicit Gallerie(QWidget *parent = nullptr);
    ~Gallerie();
    void raffrachir(std::vector<Descripteur*>* descripteurs);
    void vider();

private:
    // QScrollArea* scrollArea;
    QWidget* container;
    QGridLayout* layout;

signals:
};

#endif // GALLERIE_H



// #include <QDir>
// #include <QFileInfoList>

// class ImageGallery : public QWidget {
// public:
//     ImageGallery(const QStringList &imagePaths, QWidget *parent = nullptr) : QWidget(parent) {
//         QScrollArea *scrollArea = new QScrollArea(this);
//         QWidget *container = new QWidget;
//         QGridLayout *gridLayout = new QGridLayout(container);

//         int row = 0, col = 0;
//         for (const QString &path : imagePaths) {
//             ImageButtonWidget *imageButtonWidget = new ImageButtonWidget(path);
//             gridLayout->addWidget(imageButtonWidget, row, col);
//             connect(imageButtonWidget, &ImageButtonWidget::buttonClicked, [path]() {
//                 qDebug() << "Button clicked for image:" << path;
//             });

//             if (++col >= 3) {  // Adjust column count for layout
//                 col = 0;
//                 ++row;
//             }
//         }

//         container->setLayout(gridLayout);
//         scrollArea->setWidget(container);
//         QVBoxLayout *mainLayout = new QVBoxLayout;
//         mainLayout->addWidget(scrollArea);
//         setLayout(mainLayout);
//     }
// };
