#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T12:54:16
#
#-------------------------------------------------

TARGET      = MetaBotServer

DESTDIR     = build/
RCC_DIR     = build/
UI_DIR      = build/
MOC_DIR     = build/
OBJECTS_DIR = build/

# Enables C++11
QMAKE_CXXFLAGS += -std=c++11 -Wno-unused
QMAKE_LFLAGS   += -Wl,-rpath=/usr/local/lib

# Add SMFL to support gamepads and network
LIBS += -lsfml-window -lsfml-network

#SmartServoFramework for Dynamixel servos
#LIBS += -lSmartServoFramework
LIBS += -llockdev

# SmartServoFramework sources (if not using it as a library)
SOURCES += src/SmartServoFramework/src/*.cpp
HEADERS += src/SmartServoFramework/src/*.h

# Server application sources
SOURCES += src/main.cpp \
           src/ctrl_network.cpp \
           src/ctrl_gamepad.cpp \
           src/ctrl_keyboard.cpp \
           src/metabot_control.cpp \
           src/metabot_kinematic.cpp \
           src/metabot_function.cpp

HEADERS += src/minitraces_conf.h \
           src/ctrl.h \
           src/ctrl_network.h \
           src/ctrl_gamepad.h \
           src/ctrl_keyboard.h \
           src/metabot_config.h \
           src/metabot_control.h \
           src/metabot_kinematic.h \
           src/metabot_function.h
