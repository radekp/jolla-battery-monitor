#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QObject>
#include <QDateTime>

class BatteryMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BatteryMonitor(QObject *parent = 0);

    static BatteryMonitor * getInstance()
    {
        static BatteryMonitor instance;
        return &instance;
    }

    QString chargeLog;

    int currentNow;

    // Values used to compute current since last charge reading
    QDateTime lastDt;
    int lastCharge;

    // We use current_now to approximate charge log until charge_now changes
    int approxLogStart;
    int approxCharge;
    QDateTime approxDt;

signals:

public slots:
    QString readCharge();
    QString readChargeCover();
};

#endif // BATTERYMONITOR_H
