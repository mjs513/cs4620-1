TEMPLATE = app
TARGET = cs4620
QT += core \
    opengl \
    gui
HEADERS += src/Joint.h \
    src/SphereCamera.h \
    src/Color.h \
    src/GLWidget.h \
    src/MainWindow.h \
    src/Matrix.h \
    src/OpenGL.h \
    src/Plane.h \
    src/Point.h \
    src/Random.h \
    src/Rectangle.h \
    src/Vector.h
SOURCES += src/Joint.cpp \
    src/SphereCamera.cpp \
    src/Color.cpp \
    src/GLWidget.cpp \
    src/MainWindow.cpp \
    src/Matrix.cpp \
    src/OpenGL.cpp \
    src/Plane.cpp \
    src/Point.cpp \
    src/Random.cpp \
    src/Rectangle.cpp \
    src/Vector.cpp \
    src/main.cpp
FORMS += cs4620.ui
RESOURCES += 
