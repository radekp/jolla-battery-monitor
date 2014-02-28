#include "chargegraph.h"
#include "chargeutils.h"
#include "batterymonitor.h"

ChargeGraph::ChargeGraph(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void ChargeGraph::paint(QPainter *p)
{
    BatteryMonitor *mon = BatteryMonitor::getInstance();
    QList < QString > lines = mon->chargeLog.split('\n');
    if (lines.count() < 2)
        return;

    QList < QDateTime > dates;
    QList < int >charges;

    QDateTime dtMin(QDate(2999, 1, 1));
    QDateTime dtMax(QDate(1899, 1, 1));
    int chargeMax = 0;

    for (int i = 0; i < lines.count(); i++) {
        QList < QString > values = lines.at(i).split('\t');
        if (values.count() < 2) {
            continue;
        }
        QDateTime dt =
            QDateTime::fromString(values.at(0), "yyyy-MM-dd hh:mm:ss");
        int charge = values.at(1).toInt();
        dates.append(dt);
        charges.append(charge);

        dtMin = (dtMin < dt ? dtMin : dt);
        dtMax = (dtMax > dt ? dtMax : dt);
        chargeMax = (chargeMax > charge ? chargeMax : charge);
    }

    int fontW = p->fontMetrics().width('w');
    int fontH = p->fontMetrics().height();

    int w = (9 * width()) / 10;
    int h = height() - (5 * fontH) / 2;

    int totalSecs = dtMin.secsTo(dtMax);

    if (chargeMax == 0 || totalSecs == 0)
        return;

    //p->fillRect(0, 0, width(), height(), Qt::black);
    p->setBackground(Qt::black);
    p->setBrush(Qt::white);
    p->setPen(Qt::white);
    p->setFont(QFont(p->font().family(), 11, QFont::Bold));

    p->translate(fontW, fontH);

    p->drawLine(0, h, w, h);
    p->drawLine(0, 0, 0, h);

    QPen pen = p->pen();

    for (int round = 0; round <= 1; round++) {
        int chargeX = 0x7fffffff;
        int currentX = 0x7fffffff;
        int hourTextX = 0x7fffffff;

        int x1 = -1;
        int y1 = -1;
        int prevSecs = -1;
        int prevCharge = -1;

        for (int i = 0; i < dates.count(); i++) {
            QDateTime dt = dates.at(i);
            int charge = charges.at(i);
            int secs = dtMin.secsTo(dt);

            int x2 = (w * secs) / totalSecs;
            int y2 = (h * charge) / chargeMax;

            y2 = h - (5 * y2 / 6);      // flip y and add 1/6 for charge value text

            // Draw time on x axis
            if (abs(x2 - hourTextX) > 5 * fontW) {
                if (round == 0) {
                    p->drawText(x2, h + fontH, dt.toString("hh:mm"));
                    p->setPen(Qt::darkGreen);
                    p->drawLine(x2, y2, x2, h);
                    p->setPen(pen);
                }
                hourTextX = x2;
            }
            // Draw charge point and charge value
            p->drawEllipse(x2 - 2, y2 - 2, 4, 4);
            int shiftY = -h / 6;
            int y = y2 + shiftY;
            if (abs(x2 - chargeX) > 2 * fontW) {
                QString text = QString::number(charge);

                p->setPen(Qt::darkGreen);
                p->drawLine(x2, y, x2, y2);
                p->setPen(pen);

                p->save();
                p->translate(x2, y);
                p->rotate(90);
                p->drawText(0, 0, text);
                p->restore();

                chargeX = x2;
            }
            // Draw charge line and in the middle write current
            if (x1 >= 0) {
                p->drawLine(x1, y1, x2, y2);
                y = (y1 + y2) / 2;
                int x = (x1 + x2) / 2;
                int shiftY = h / 4;
                if (abs(x - currentX) > 2 * fontW) {
                    int current =
                        computeCurrent(secs - prevSecs, prevCharge, charge);
                    if (round == 0) {
                        p->setPen(Qt::red);
                        p->drawLine(x, y, x, y + shiftY);
                        p->setPen(pen);
                    }
                    QString text = QString::number(current) + "mA";
                    int textW = p->fontMetrics().width(text);
                    p->save();
                    p->translate(x, y + shiftY);
                    p->rotate(90);
                    p->fillRect(0, -fontH, textW, fontH, Qt::red);
                    p->drawText(0, 0, text);
                    p->restore();
                    currentX = x;
                }
            }

            x1 = x2;
            y1 = y2;
            prevSecs = secs;
            prevCharge = charge;
        }
    }
}
