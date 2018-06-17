#-------------------------------------------------
# MetaBot Server
#-------------------------------------------------

TARGET      = MetaBotServer

CONFIG     += c++11

DESTDIR     = build/
RCC_DIR     = build/
UI_DIR      = build/
MOC_DIR     = build/
OBJECTS_DIR = build/

# SmartServoFramework
contains(CONFIG, SSF_SYSTEM) {
    # Use SmartServoFramework library from the system
    unix {
        INCLUDEPATH += `pkg-config --cflags smartservoframework`
        LIBS += `pkg-config --libs smartservoframework`
    }
    win32 {
        SSF_DIR = ../SmartServoFramework
        INCLUDEPATH += $${SSF_DIR}
        LIBS += $${SSF_DIR}/build/smartservoframework.lib
    }
} else {
    # Use SmartServoFramework sources directly
    SSF_DIR = ../SmartServoFramework
    INCLUDEPATH += $${SSF_DIR}
    SOURCES += $${SSF_DIR}/SmartServoFramework/C*.cpp
    SOURCES += $${SSF_DIR}/SmartServoFramework/D*.cpp
    SOURCES += $${SSF_DIR}/SmartServoFramework/H*.cpp
    SOURCES += $${SSF_DIR}/SmartServoFramework/S*.cpp
    HEADERS += $${SSF_DIR}/SmartServoFramework/*.h
}

# Add SMFL to support gamepads and network
LIBS += -lsfml-window -lsfml-network


# MetaBot Server application sources
SOURCES += src/main.cpp \
           src/minitraces.c \
           src/ctrl_network.cpp \
           src/ctrl_gamepad.cpp \
           src/ctrl_keyboard.cpp \
           src/metabot_control.cpp \
           src/metabot_kinematic.cpp \
           src/metabot_function.cpp

HEADERS += src/minitraces.h \
           src/minitraces_conf.h \
           src/ctrl.h \
           src/ctrl_network.h \
           src/ctrl_gamepad.h \
           src/ctrl_keyboard.h \
           src/metabot_config.h \
           src/metabot_control.h \
           src/metabot_kinematic.h \
           src/metabot_function.h
