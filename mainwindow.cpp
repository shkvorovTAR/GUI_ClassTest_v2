#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QtMath>

#define MAX_W 800
#define MAX_H 800

#define HALF_W MAX_W / 2
#define HALF_H MAX_H / 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    setFixedSize(MAX_W+100, MAX_H+100);
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(-HALF_W, -HALF_H, MAX_W, MAX_H);

    setupScene();
}

void MainWindow::setupScene()
{
    scene->setBackgroundBrush(QBrush(QImage(":/images/galaxy_starfield.png")));
    setupPlanets();
    positionAll();
}

void MainWindow::setupPlanets()
{
    sun = createPlanet("sun");
    applyAe(sun);

    for(int i=0; i<4; i++)
    {
        items[i*2] = createPlanet(itemNames[i]);
        items[i*2+1] = createOrbit(i);
    }
}

QGraphicsItem *MainWindow::createPlanet(QString name)
{
    QString path = QString(":/images/%1.png").arg(name);
    QGraphicsItem *item = scene->addPixmap(QPixmap(path));
    return item;
}

QGraphicsItem *MainWindow::createOrbit(int i)
{
    QRectF itemRect = sun->mapRectToScene(sun->boundingRect());
    double diameter = (ae * itemsKoeffs[i*2] ) * 2 + itemRect.width();
    QRectF rect = QRectF(-diameter / 2.0, -diameter / 2.0, diameter, diameter);
    QGraphicsItem *item = scene->addEllipse(rect, QPen(Qt::white));
    item->setZValue(-1);
    return item;
}

void MainWindow::applyAe(QGraphicsItem *item)
{
    QRectF itemRect = sun->mapRectToScene(sun->boundingRect());
    item->setScale(ae / 1000.0);
    QTransform transform;
    transform.translate(-itemRect.width()/2, -itemRect.height()/2);
    item->setTransform(transform);
}

void MainWindow::positionAll()
{
    applyAe(sun);

    for(int i=0; i<4; i++) {
        positionPlanet(items[i*2], itemsKoeffs[i*2], itemsKoeffs[i*2+1]);
        QGraphicsItem *orbit = items[i*2+1];
        if(orbit != nullptr) {
            scene->removeItem(orbit);
        }
        items[i*2+1] = createOrbit(i);
    }
}

void MainWindow::positionPlanet(QGraphicsItem *item, double k, double d)
{
    double degrees = day * d;
    double alpha = qDegreesToRadians((qreal) degrees);
    double x = qCos(alpha) * k * ae;
    double y = -qSin(alpha) * k * ae;
    //applyAe(item);
    item->setScale(ae / 1000.0);
    item->setPos(x, y);
    item->setRotation(-degrees);
}

void MainWindow::timerTick()
{
    ui->spinBox->setValue(day + 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    day = arg1;
    positionAll();
}

void MainWindow::on_pushButton_clicked()
{
    if(timer->isActive()) {
        ui->pushButton->setText("Start");
        timer->stop();
    } else {
        ui->pushButton->setText("Stop");
        timer->start(1000/1);
    }
}

void MainWindow::on_horizontalSlider_AE_valueChanged(int value)
{
    ae = value;
    ui->label_AE->setText(QString("AE = %1").arg(ae));
    positionAll();
}
