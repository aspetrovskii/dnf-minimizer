TEMPLATE = app
TARGET = dnf_minimizer
CONFIG += c++17
QT += widgets
SOURCES += main.cpp \
    minimizersession.cpp
HEADERS += minimizersession.h
INCLUDEPATH += $$PWD/../backend
LIBS += -L$$OUT_PWD/../backend -lminimizer_core
