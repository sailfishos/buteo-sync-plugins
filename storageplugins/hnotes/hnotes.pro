TEMPLATE = lib
TARGET = hnotes-storage
DEPENDPATH += .
INCLUDEPATH += . ../../syncmlcommon

LIBS += -L../../syncmlcommon

CONFIG += link_pkgconfig plugin

PKGCONFIG = buteosyncfw5 KF5CalendarCore libmkcal-qt5
LIBS += -lsyncmlcommon5
target.path = $$[QT_INSTALL_LIBS]/buteo-plugins-qt5

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0

QT -= gui

#input
HEADERS += NotesStorage.h \
           NotesBackend.h \

SOURCES += NotesStorage.cpp \
           NotesBackend.cpp \

QMAKE_CXXFLAGS = -Wall \
    -g \
    -Wno-cast-align \
    -O2 -finline-functions


#clean
QMAKE_CLEAN += $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
QMAKE_CLEAN += $(OBJECTS_DIR)/*.gcda $(OBJECTS_DIR)/*.gcno $(OBJECTS_DIR)/*.gcov $(OBJECTS_DIR)/moc_*

ctcaps.path =/etc/buteo/xml/
ctcaps.files=xml/CTCaps_notes_11.xml xml/CTCaps_notes_12.xml

INSTALLS += target ctcaps
