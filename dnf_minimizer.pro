TEMPLATE = app
TARGET = dnf_minimizer
CONFIG += c++17
QT += widgets
SOURCES += backend/minimizer.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/minimizersession.cpp \
    src/mindnf_formatter.cpp \
    src/funccolumnwidget.cpp \
    src/setspanelwidget.cpp \
    src/centralminimizerwidget.cpp
HEADERS += backend/minimizer.h \
    src/minimizersession.h \
    src/mindnf_formatter.h \
    src/mainwindow.h \
    src/funccolumnwidget.h \
    src/setspanelwidget.h \
    src/centralminimizerwidget.h
INCLUDEPATH += $$PWD/backend $$PWD/src
