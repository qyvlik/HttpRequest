TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    src/httprequest.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/httprequest.h \
    src/httprequest_p.h

CONFIG += c++11

OTHER_FILES += *.md
