#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QObject>

class BatteryMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BatteryMonitor(QObject *parent = 0);

signals:

public slots:
    QString infoString();

};

#endif // BATTERYMONITOR_H
