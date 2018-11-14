QT += widgets

HEADERS     = dialog.h \
    mywidget.h \
    style.h \
    wdg.h
SOURCES     = dialog.cpp \
              main.cpp \
    mywidget.cpp \
    wdg.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/layouts/basiclayouts
INSTALLS += target

CONFIG += exception;

DISTFILES += \
    style.qss
