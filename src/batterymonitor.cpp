#include "batterymonitor.h"
#include "chargeutils.h"

#include <QFile>
#include <QPainter>
#include <QList>
#include <QDateTime>

BatteryMonitor::BatteryMonitor(QObject *parent) :
    QObject(parent)
{
    //chargeLog = "2014-02-11 10:00:00\t1000000\n2014-02-11 11:00:00\t950000\n2014-02-11 12:00:00\t850000\n";
}

// Parse uevent string and return given attribute value. Example uevent file:
// POWER_SUPPLY_NAME=bq27000-battery
// POWER_SUPPLY_STATUS=Full
// POWER_SUPPLY_PRESENT=1
// POWER_SUPPLY_VOLTAGE_NOW=65535000
// POWER_SUPPLY_CURRENT_NOW=-11697997
// POWER_SUPPLY_CAPACITY_LEVEL=Full
// POWER_SUPPLY_TEMP=161106
// POWER_SUPPLY_TECHNOLOGY=Li-ion
// POWER_SUPPLY_CHARGE_NOW=11697997
// POWER_SUPPLY_CHARGE_FULL_DESIGN=11652480
// POWER_SUPPLY_CYCLE_COUNT=65535
//
// Btw this is uevent file for case of uncalibrated battery.
//
// The argument name must contain even the '=' char
static QByteArray getAttr(const char *name, QByteArray & uevent)
{
    int index = uevent.indexOf(name);
    if (index < 0)
        return QByteArray();

    index += strlen(name);
    int end = uevent.indexOf('\n', index + 1);
    return uevent.mid(index, end - index);
}

static int getIntAttr(const char *name, QByteArray & uevent)
{
    QByteArray str = getAttr(name, uevent);
    if (str.isEmpty())
        return -1;
    return str.toInt();
}

QString mkLogLine(QDateTime now, int chargeNow)
{
    return
        now.toString("yyyy-MM-dd hh:mm:ss") + "\t" +
        QString::number(chargeNow) + "\n";
}

QString BatteryMonitor::readCharge()
{
#if 0
    QByteArray content = "POWER_SUPPLY_CHARGE_NOW=" + QByteArray::number(QDateTime::currentDateTime().toTime_t() % 2100000);
#else
    QFile f("/sys/class/power_supply/battery/uevent");
    if (!f.open(QFile::ReadOnly))
        return "failed to open battery uevent: " + f.errorString();

    QByteArray content = f.readAll();
    f.close();
#endif

    // Actual current - it updates quite often, but we can recompute it with
    // precise value when charge_now changes
    currentNow = getIntAttr("POWER_SUPPLY_CURRENT_NOW=", content);

    QDateTime now = QDateTime::currentDateTime();
    int chargeNow = getIntAttr("POWER_SUPPLY_CHARGE_NOW=", content);

    if(chargeNow != lastCharge) {
        chargeLog = chargeLog.left(approxLogStart);       // remove approximated log part
        chargeLog += mkLogLine(now, chargeNow);
        approxLogStart = chargeLog.length();

        // Compute precise current since lastDt
        currentNow = computeCurrent(lastDt.secsTo(now), lastCharge, chargeNow);

        lastCharge = approxCharge = chargeNow;
        lastDt = now;
    }
    else {
        // Use current_now which updates more often to guess discharging curve
        approxCharge = computeCharge(approxDt.secsTo(now), currentNow, approxCharge);
        chargeLog += mkLogLine(now, approxCharge);
    }
    approxDt = now;

    content = content.replace("POWER_SUPPLY_", "");

    return content;
}

QString BatteryMonitor::readChargeCover()
{
    // Refresh battery info from uevent file
    readCharge();

    return "Battery monitor\nCharge: "
            + QString::number(lastCharge / 1000) + "mAh\nCurrent: "
            + QString::number(currentNow / 1000) + "mA";
}
