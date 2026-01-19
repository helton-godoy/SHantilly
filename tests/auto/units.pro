QT += testlib core widgets
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_units

# Path to SHantilly source code (from tests/auto/)
SHANTILLY_SRC = ../../src/code/SHantilly

SOURCES += tst_units.cpp \
           $$SHANTILLY_SRC/execution_context.cpp \
           $$SHANTILLY_SRC/logger.cpp

HEADERS += $$SHANTILLY_SRC/execution_context.h \
           $$SHANTILLY_SRC/logger.h

INCLUDEPATH += $$SHANTILLY_SRC

CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Wextra
