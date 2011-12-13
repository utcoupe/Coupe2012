#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rampe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Rampe rampe;

    // recul
    // v0 == 0;
    // vf == 0
    rampe.compute(500000, 0, -2000, 50, -50, 0, 0);

    // recul
    // v0 != 0
    // vf == 0
    //rampe.compute(500000, 0, -2000, 50, -50, -1000, 0);

    // recul
    // v0 == 0
    // vf != 0
    //rampe.compute(500000, 0, -2000, 50, -50, 0, -1000);

    // recul
    // |v0| > |vmax|
    // vf == 0
    //rampe.compute(500000, 0, -2000, 50, -50, -5000, 0);

    // recul
    // |v0| > |vmax|
    // vf != 0
    //rampe.compute(500000, 0, -2000, 50, -50, -5000, -1000);

    // recul
    // trop court
    // v0 == 0
    // vf == 0
    //rampe.compute(50000, 0, -2000, 100, -50, 0, 0);

    // recul
    // trop court
    // v0 != 0
    // vf == 0
    //rampe.compute(50000, 0, -2000, 100, -50, -1000, 0);

    // recul
    // trop court
    // v0 == 0
    // vf != 0
    //rampe.compute(50000, 0, -2000, 100, -50, 0, -1000);

    // recul
    // trop court
    // |v0| > |vmax|
    // vf == 0
    //rampe.compute(50000, 0, -2000, 50, -50, -5000, 0);

    // recul
    // trop court
    // |v0| > |vmax|
    // vf != 0
    //rampe.compute(50000, 0, -2000, 50, -50, -5000, -1000);


    //rampe.update_speedf(-1000);

    int i = 0;
    double a;
    static const int DT = 5;
    while (rampe.get_phase() != PHASE_END) {
        a = rampe.get_speed();
        ui->renderSpeed->addPoint(QPointF(i,a));
        a = rampe.get_pos();
        ui->renderGoal->addPoint(QPointF(i,a));
        i += DT;
        rampe.compute_next_goal(DT);
    }
    a = rampe.get_speed();
    ui->renderSpeed->addPoint(QPointF(i,a));
    a = rampe.get_pos();
    ui->renderGoal->addPoint(QPointF(i,a));

}

MainWindow::~MainWindow()
{
    delete ui;
}
