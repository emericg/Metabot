#-------------------------------------------------
# MetaBot Client
#-------------------------------------------------

TARGET      = MetaBotClient
TEMPLATE    = app

CONFIG     += c++11
CONFIG     += qt
QT         += core gui widgets

DESTDIR     = build/
RCC_DIR     = build/
UI_DIR      = build/
MOC_DIR     = build/
OBJECTS_DIR = build/

# Add SMFL to support gamepads and network
LIBS += -lsfml-window -lsfml-network -lsfml-system

# Client application sources
SOURCES += src/main.cpp \
           src/metabotgui.cpp \
           src/network.cpp \
           ../metabot_server/src/minitraces.c \
           ../metabot_server/src/ctrl_gamepad.cpp \
           ../metabot_server/src/ctrl_keyboard.cpp

HEADERS += src/metabotgui.h \
           src/network.h \
           ../metabot_server/src/minitraces.h \
           ../metabot_server/src/minitraces_conf.h \
           ../metabot_server/src/ctrl.h \
           ../metabot_server/src/ctrl_gamepad.h \
           ../metabot_server/src/ctrl_keyboard.h

FORMS   += ui/metabotgui.ui
