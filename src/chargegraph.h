#ifndef CHARGEGRAPH_H
#define CHARGEGRAPH_H

#include <QtQuick>

class ChargeGraph : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit ChargeGraph(QQuickItem *parent = 0);
    void paint(QPainter *painter);

signals:

public slots:

};

#endif // CHARGEGRAPH_H
