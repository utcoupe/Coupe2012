#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QVector>
#include <QPointF>
#include <QWidget>


class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    void addPoint(const QPointF point);
    void addPoint(qreal x, qreal y);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QVector<QPointF> points;
};

#endif // RENDERAREA_H
