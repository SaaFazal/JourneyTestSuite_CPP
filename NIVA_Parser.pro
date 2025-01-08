TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++20 -Wall -Wfatal-errors

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = niva-parser-tests

LIBS += -lboost_unit_test_framework

INCLUDEPATH += headers/

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/niva/niva-data_reading.h \
    headers/niva/niva-parser.h \
    headers/types.h \
    headers/waypoint.h

SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/niva/niva-parser.cpp \
    src/waypoint.cpp

SOURCES += \
    tests/niva/BoostUTF-main.cpp \
    tests/niva/niva-parser-extractWaypointFromReading-tests.cpp \
    tests/niva/niva-parser-extractWaypointsFromLog-tests.cpp \
    tests/niva/niva-parser-hasCorrectNumberOfFields-tests.cpp \
    tests/niva/niva-parser-isWellformedDataReading-tests.cpp \
    tests/niva/niva-parser-isKnownFormat-tests.cpp \
    tests/niva/niva-parser-parseDataReading-tests.cpp \
    tests/niva/niva-parser-computeChecksum-tests.cpp \
    tests/niva/niva-parser-hasMatchingChecksum-tests.cpp
