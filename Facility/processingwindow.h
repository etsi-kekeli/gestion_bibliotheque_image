#ifndef PROCESSINGWINDOW_H
#define PROCESSINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class ProcessingWindow;
}

class ProcessingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessingWindow(QWidget *parent = nullptr);
    ~ProcessingWindow();

private slots:
    void on_SeuilDial_valueChanged(int value);

    void on_RestValRGBSeuilButton_clicked();

    void on_actionOuvrir_triggered();

    void on_actionSortir_triggered();

private:
    Ui::ProcessingWindow *ui;
};

#endif // PROCESSINGWINDOW_H
