#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rampe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Rampe rampe;
    rampe.compute(0, 30000, 500, 20, -20, 200);

    int i = 0;
    double a;
    static const int DT = 1;
    while (rampe.get_phase() != PHASE_END) {
        a = rampe.get_speed();
        ui->renderSpeed->addPoint(QPointF(i,a));
        a = rampe.get_goal();
        ui->renderGoal->addPoint(QPointF(i,a));
        i += DT;
        rampe.compute_next_goal(DT);
    }
    a = rampe.get_speed();
    ui->renderSpeed->addPoint(QPointF(i,a));
    a = rampe.get_goal();
    ui->renderGoal->addPoint(QPointF(i,a));

}

MainWindow::~MainWindow()
{
    delete ui;
}
