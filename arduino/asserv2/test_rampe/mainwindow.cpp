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
    // v0 == 0
    // vf == 0
    //
    //   vm ---- vm
    //  /         \
    // v0          vf
    //rampe.compute(500000, 0, -2000, 50, -50, 0, 0);

    // recul
    // v0 != 0
    // vf == 0
    //
    //   vm ---- vm
    //  /         \
    // v0          \
    //              vf
    //rampe.compute(500000, 0, -2000, 50, -50, -1000, 0);

    // recul
    // v0 == 0
    // vf != 0
    //
    //    vm ---- vm
    //   /          \
    //  /            vf
    // v0
    //rampe.compute(500000, 0, -2000, 50, -50, 0, -1000);

    // recul
    // |v0| > |vmax|
    // vf == 0
    //
    // v0
    //  \
    //   vm ---- vm
    //             \
    //              vf
    //rampe.compute(500000, 0, -2000, 50, -50, -5000, 0);

    // recul
    // |v0| > |vmax|
    // vf != 0
    //
    // v0
    //  \
    //   vm ---- vm
    //             \
    //              vf
    //
    // 0 - - - - - - 0
    //rampe.compute(500000, 0, -2000, 50, -50, -5000, -1000);

    // recul
    // trop court
    // v0 == 0
    // vf == 0
    //
    //    vm
    //  /    \
    // v0     vf
    //rampe.compute(50000, 0, -2000, 100, -50, 0, 0);

    // recul
    // trop court
    // v0 != 0
    // vf == 0
    //
    //    vm
    //  /    \
    // v0     \
    //        vf
    //rampe.compute(50000, 0, -2000, 100, -50, -1000, 0);

    // recul
    // trop court
    // v0 == 0
    // vf != 0
    //
    //     vm
    //   /    \
    //  /      vf
    // v0
    //rampe.compute(50000, 0, -2000, 100, -50, 0, -1000);

    // recul
    // trop court
    // |v0| > |vmax|
    // vf == 0
    //
    // v0
    //   \
    //    vf
    //rampe.compute(50000, 0, -2000, 50, -50, -5000, 0);

    // recul
    // trop court
    // |v0| > |vmax|
    // vf != 0
    //
    // v0
    //   \
    //    vf
    //
    // 0 - - - - - - 0
    //rampe.compute(50000, 0, -2000, 50, -50, -5000, -1000);

    // recul
    // trop court
    // v0 == 0
    // |vf| > |vmax|
    //rampe.compute(50000, 0, -2000, 50, -50, 0, -4000);


    //rampe.update_speedf(-1000);

    //rampe.compute(4681000, 0, -9360, -1000, 1000, 0, 0);
    rampe.compute(4681, 0, -9.36, -1.0, 1.0, 0, 0);

    int i = 0;
    double a;
    static const int DT = 2;
    while (rampe.get_phase() != PHASE_END) {
        a = rampe.get_speed();
        ui->renderSpeed->addPoint(QPointF(i,a));
        a = rampe.get_pos();
        ui->renderGoal->addPoint(QPointF(i,a));
        i += DT;
        /*if (i==100)
            rampe.update_speedf(-1000);*/
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
