TEMPLATE = app
TARGET = hcalendar-tests
DEPENDPATH += . \
              ../ \
              ../../../syncmlcommon

VPATH = .. \
    ../../../syncmlcommon

INCLUDEPATH += . \
    ../ \
    ../../../syncmlcommon

HEADERS += CalendarTest.h \
           CalendarStorage.h \
           CalendarBackend.h \
           SimpleItem.h \
           SyncMLConfig.h

SOURCES += CalendarTest.cpp \
           CalendarStorage.cpp \
           CalendarBackend.cpp \
           SimpleItem.cpp \
           SyncMLConfig.cpp

CONFIG += link_pkgconfig
LIBS += -L../../../syncmlcommon

PKGCONFIG = buteosyncfw5 KF5CalendarCore libmkcal-qt5
LIBS += -lsyncmlcommon5

QT += testlib \
    core \
    network \
    xml \
    sql
QT -= gui

target.path = /opt/tests/buteo-sync-plugins/

INSTALLS += target
