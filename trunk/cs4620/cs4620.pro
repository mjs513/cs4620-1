TEMPLATE = app
TARGET = cs4620
QT += core \
    opengl \
    gui
HEADERS += src/Rectangle.h \
    src/Matrix.h \
    src/Point.h \
    src/Vector.h \
    src/Color.h \
    src/OpenGL.h \
    src/Pointer.h \
    src/GLWidget.h \
    src/MainWindow.h \
    src/opengl_skel.h
SOURCES += src/Rectangle.cpp \
    src/Matrix.cpp \
    src/Point.cpp \
    src/Vector.cpp \
    src/Color.cpp \
    src/OpenGL.cpp \
    src/GLWidget.cpp \
    src/MainWindow.cpp \
    src/main.cpp
FORMS += cs4620.ui
RESOURCES += 
