#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QObject>
#include <QImage>

class BatteryMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BatteryMonitor(QObject *parent = 0);

private:
    QString chargeLog;

signals:

public slots:
    QString infoString();
    QImage chargeGraph(int, int);

};

#endif // BATTERYMONITOR_H
