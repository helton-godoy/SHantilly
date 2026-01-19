QT += testlib core widgets
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_commands

# Path to SHantilly source code (from tests/auto/)
SHANTILLY_SRC = ../../src/code/SHantilly

SOURCES += tst_commands.cpp \
           $$SHANTILLY_SRC/commands/add_command.cpp \
           $$SHANTILLY_SRC/commands/set_command.cpp \
           $$SHANTILLY_SRC/commands/command_utils.cpp \
           $$SHANTILLY_SRC/execution_context.cpp \
           $$SHANTILLY_SRC/logger.cpp

HEADERS += $$SHANTILLY_SRC/commands/add_command.h \
           $$SHANTILLY_SRC/commands/set_command.h \
           $$SHANTILLY_SRC/commands/command_utils.h \
           $$SHANTILLY_SRC/execution_context.h \
           $$SHANTILLY_SRC/command.h \
           $$SHANTILLY_SRC/logger.h

INCLUDEPATH += $$SHANTILLY_SRC

CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Wextra
