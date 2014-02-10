#include "batterymonitor.h"
#include <QFile>

BatteryMonitor::BatteryMonitor(QObject *parent) :
    QObject(parent)
{
}

QString BatteryMonitor::infoString()
{
    QFile f("/sys/class/power_supply/battery/uevent");
    if (!f.open(QFile::ReadOnly))
        return "failed to open battery uevent: " + f.errorString();

    QByteArray content = f.readAll();
    content = content.replace("POWER_SUPPLY_", "");
    f.close();

    return content;
}
