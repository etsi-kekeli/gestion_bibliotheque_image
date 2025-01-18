#ifndef PROCESSINGWINDOW_H
#define PROCESSINGWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QImage>
#include <opencv2/opencv.hpp>

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

    void on_actionSauvegarder_triggered();

    void on_segRGBButton_clicked();

    void on_segmentHSV_clicked();

    void on_teinteButton_clicked();

private:
    Ui::ProcessingWindow *ui;
    cv::Mat originalImage;
    QGraphicsScene *sceneOriginal;
    QGraphicsScene *sceneResult;

    void displayImage(const cv::Mat& mat, QGraphicsView* view, QGraphicsScene** scene);
    QImage matToQImage(const cv::Mat& mat);
};

#endif // PROCESSINGWINDOW_H
