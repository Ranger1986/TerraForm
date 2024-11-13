MOC_DIR = ./moc
OBJECTS_DIR = ./obj

INCLUDEPATH += $$PWD

HEADERS       = glwidget.h \
                glcontainer.h \
                mesh.h \
                paintwidget.h \
                window.h \
                mainwindow.h \
                logo.h
SOURCES       = glwidget.cpp \
                glcontainer.cpp \
                main.cpp \
                mesh.cpp \
                paintwidget.cpp \
                window.cpp \
                mainwindow.cpp \
                logo.cpp

RESOURCES += \
    shaders.qrc

QT           += widgets
#QT           += opengl
#QT           += openglwidgets

DISTFILES += \
    Bassae.png \
    README.md


