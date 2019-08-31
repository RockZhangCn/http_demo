TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    request.cpp \
    common.cpp \
    response.cpp

HEADERS += \
    request.h \
    common.h \
    response.h
