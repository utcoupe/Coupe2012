#include <QtGui>

#include "renderarea.h"


RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea::addPoint(const QPointF point)
{
    points.push_back(point);
}

void RenderArea::addPoint(qreal x, qreal y)
{
    points.push_back(QPointF(x,y));
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    int width = 1;

    Qt::PenStyle style(Qt::SolidLine);
    Qt::PenCapStyle cap(Qt::FlatCap);
    Qt::PenJoinStyle join(Qt::RoundJoin);
    QPen pen(Qt::blue, width, style, cap, join);

    QBrush brush(Qt::green, Qt::NoBrush);

    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    // lisser les tracés
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal maxX = 0;
    qreal maxY = 0;
    for(QVector<QPointF>::iterator it = points.begin(); it != points.end(); ++it) {
        maxX = std::max(maxX, it->x());
        maxY = std::max(maxY, it->y());
    }

    static const int MARGE = 5;
    qreal scaleX = (qreal) (this->width()-2*MARGE) / (qreal) maxX;
    qreal scaleY = (qreal) (this->height()-2*MARGE) / (qreal) maxY;

    QPainterPath path;
    if (!points.isEmpty())
        path.moveTo((qreal)MARGE + scaleX * points.first().x(), (qreal)MARGE + scaleY * points.first().y());
    for(QVector<QPointF>::iterator it = points.begin(); it != points.end(); ++it)
        path.lineTo((qreal)MARGE + scaleX * it->x(), (qreal)MARGE + scaleY * it->y());

    painter.drawPath(path);

}
