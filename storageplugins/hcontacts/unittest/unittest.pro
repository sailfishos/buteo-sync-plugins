TEMPLATE = app
TARGET = hcontacts-tests

QT -= gui
QT += core testlib sql
CONFIG += link_pkgconfig

PKGCONFIG = buteosyncfw5 Qt5Contacts Qt5Versit buteosyncml5 qtcontacts-sqlite-qt5-extensions contactcache-qt5
LIBS += -lsyncmlcommon5

DEPENDPATH += . \
              ../ \

VPATH = .. \
    ../../../

INCLUDEPATH += . \
    ../ \
    ../../../syncmlcommon

LIBS += -L../../../syncmlcommon

HEADERS += ContactsTest.h \
           ContactsStorage.h \
           ContactsBackend.h \
           ContactBuilder.h

SOURCES += ContactsTest.cpp \
           ContactsStorage.cpp \
           ContactsBackend.cpp \
           ContactBuilder.cpp

QMAKE_CLEAN += $(OBJECTS_DIR)/*.gcda $(OBJECTS_DIR)/*.gcno
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
#install
testfiles.path = /opt/tests/buteo-sync-plugins/
testfiles.files =  hcontacts-tests.ref vcard1.txt vcard2.txt vcard3.txt

target.path = /opt/tests/buteo-sync-plugins/
INSTALLS += target \
            testfiles
