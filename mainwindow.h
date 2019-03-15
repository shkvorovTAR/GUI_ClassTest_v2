#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QPixmap>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



public slots:
    void timerTick();
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_horizontalSlider_AE_valueChanged(int value);

private:
    QString itemNames[4] = {
        "mercury", "venus", "earth", "mars"
    };

    double itemsKoeffs[8] = {
        0.387, 360/88.0,
        0.723, 360/224.0,
        1.000, 1,
        1.524, 360/687.0
    };

    QGraphicsItem *sun;
    QGraphicsItem* items[8] = {};

    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QTimer *timer;

    int day = 0;
    int ae = 200;
    void setupScene();
    void setupPlanets();
    QGraphicsItem *createPlanet(QString name);
    QGraphicsItem *createOrbit(int i);
    QRectF createOrbitRect(int i);
    void positionAll();
    void positionPlanet(QGraphicsItem *item, double k, double d);
    void applyAe(QGraphicsItem *);
};

#endif // MAINWINDOW_H
