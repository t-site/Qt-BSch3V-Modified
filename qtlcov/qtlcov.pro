######################################################################
# Automatically generated by qmake (2.01a) ? 2? 2 20:38:14 2010
######################################################################

QT = core gui widgets
RC_FILE = qtlcov.rc
ICON = lcov.icns
TEMPLATE = app
TARGET = qtlcov
DEPENDPATH += . ../BSch3VShared
INCLUDEPATH += . ../BSch3VShared

win32:DEFINES += _CRT_SECURE_NO_WARNINGS

unix:LIBS += -L../BSch3VShared/ -lBSch3VShared
win32:LIBS += ..\\BSch3VShared/BSch3VShared.lib

# Input
HEADERS += add_alias.h \
           application.h \
           compeditdoc.h \
           compeditelm.h \
           componentview.h \
           editptntextdlg.h \
           lcolistboxitem.h \
           lcovdoc.h \
           propcomponentdlg.h \
           proppindlg.h \
           propptn.h \
           resource.h \
           stdafx.h \
    componentlistmodel.h
FORMS += editptntextdlg.ui propcomponentdlg.ui proppindlg.ui
SOURCES += add_alias.cpp \
           application.cpp \
           compeditdoc.cpp \
           compeditelm.cpp \
           componentview.cpp \
           coord.cpp \
           editptntextdlg.cpp \
           lcolistboxitem.cpp \
           lcovdoc.cpp \
           main.cpp \
           propcomponentdlg.cpp \
           proppindlg.cpp \
           propptn.cpp \
    componentlistmodel.cpp

RESOURCES += \
    images.qrc
