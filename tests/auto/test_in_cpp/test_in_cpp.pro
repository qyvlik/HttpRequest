
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_test_in_cpp.cpp

include(../../../lib/HttpRequest/httprequest.pri)
