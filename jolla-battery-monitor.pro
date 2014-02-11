# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = jolla-battery-monitor

CONFIG += sailfishapp

SOURCES += src/jolla-battery-monitor.cpp \
    src/batterymonitor.cpp \
    src/chargegraph.cpp \
    src/chargeutils.cpp

OTHER_FILES += qml/jolla-battery-monitor.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/jolla-battery-monitor.spec \
    rpm/jolla-battery-monitor.yaml \
    jolla-battery-monitor.desktop

HEADERS += \
    src/batterymonitor.h \
    src/chargegraph.h \
    src/chargeutils.h

