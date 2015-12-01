#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T12:54:16
#
#-------------------------------------------------

TARGET      = MetaBotClient
TEMPLATE    = app
CONFIG     += qt
QT         += core gui widgets

DESTDIR     = build/
RCC_DIR     = build/
UI_DIR      = build/
MOC_DIR     = build/
OBJECTS_DIR = build/

# Enables C++11
QMAKE_CXXFLAGS += -std=c++11 -Wno-unused

# Add SMFL to support gamepads and network
LIBS += -lsfml-window -lsfml-system -lsfml-network

# Client GUI application sources
SOURCES += src/main.cpp \
           src/minitraces.c \
           src/metabotgui.cpp \
           src/network.cpp \
           src/ctrl_gamepad.cpp \
           src/ctrl_keyboard.cpp

HEADERS += src/minitraces.h \
           src/minitraces_conf.h \
           src/metabotgui.h \
           src/network.h \
           src/ctrl_gamepad.h \
           src/ctrl_keyboard.h

FORMS   += ui/metabotgui.ui
