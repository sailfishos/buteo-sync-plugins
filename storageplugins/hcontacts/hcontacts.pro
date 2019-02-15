TEMPLATE = lib
TARGET = hcontacts-storage

DEPENDPATH += .
INCLUDEPATH += .  \
    ../../syncmlcommon

CONFIG += link_pkgconfig plugin

PKGCONFIG = buteosyncfw5 Qt5Contacts Qt5Versit qtcontacts-sqlite-qt5-extensions contactcache-qt5
LIBS += -lsyncmlcommon5
target.path = /usr/lib/buteo-plugins-qt5

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0

QT -= gui
QT += sql

HEADERS += ContactsStorage.h \
           ContactsBackend.h \
           ContactBuilder.h

SOURCES += ContactsStorage.cpp \
           ContactsBackend.cpp \
           ContactBuilder.cpp


QMAKE_CXXFLAGS = -Wall \
    -g \
    -Wno-cast-align \
    -O2 -finline-functions

LIBS += -L../../syncmlcommon

QMAKE_CLEAN += $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)

ctcaps.path =/etc/buteo/xml/
ctcaps.files=xml/CTCaps_contacts_11.xml xml/CTCaps_contacts_12.xml

INSTALLS += target ctcaps
