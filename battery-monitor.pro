# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = battery-monitor

CONFIG += sailfishapp

SOURCES += src/battery-monitor.cpp \
    src/batterymonitor.cpp \
    src/chargegraph.cpp \
    src/chargeutils.cpp

OTHER_FILES += qml/battery-monitor.qml \
    rpm/battery-monitor.spec \
    rpm/battery-monitor.yaml \
    battery-monitor.desktop

HEADERS += \
    src/batterymonitor.h \
    src/chargegraph.h \
    src/chargeutils.h

