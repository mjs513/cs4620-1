TEMPLATE = app
TARGET = cs4620
QT += core \
    opengl \
    gui
HEADERS += src/GLMatrix.h \
    src/newmat/boolean.h \
    src/newmat/controlw.h \
    src/newmat/include.h \
    src/newmat/myexcept.h \
    src/newmat/newmat.h \
    src/newmat/newmatap.h \
    src/newmat/newmatio.h \
    src/newmat/newmatnl.h \
    src/newmat/newmatrc.h \
    src/newmat/newmatrm.h \
    src/newmat/precisio.h \
    src/newmat/solution.h \
    src/IKSolver.h \
    src/JointTree.h \
    src/Ray.h \
    src/Joint.h \
    src/SphereCamera.h \
    src/Color.h \
    src/GLWidget.h \
    src/MainWindow.h \
    src/OpenGL.h \
    src/Plane.h \
    src/Point.h \
    src/Random.h \
    src/Rectangle.h \
    src/Vector.h
SOURCES += src/GLMatrix.cpp \
    src/newmat/bandmat.cpp \
    src/newmat/cholesky.cpp \
    src/newmat/evalue.cpp \
    src/newmat/fft.cpp \
    src/newmat/hholder.cpp \
    src/newmat/jacobi.cpp \
    src/newmat/myexcept.cpp \
    src/newmat/newfft.cpp \
    src/newmat/newmat1.cpp \
    src/newmat/newmat2.cpp \
    src/newmat/newmat3.cpp \
    src/newmat/newmat4.cpp \
    src/newmat/newmat5.cpp \
    src/newmat/newmat6.cpp \
    src/newmat/newmat7.cpp \
    src/newmat/newmat8.cpp \
    src/newmat/newmat9.cpp \
    src/newmat/newmatex.cpp \
    src/newmat/newmatnl.cpp \
    src/newmat/newmatrm.cpp \
    src/newmat/solution.cpp \
    src/newmat/sort.cpp \
    src/newmat/submat.cpp \
    src/newmat/svd.cpp \
    src/IKSolver.cpp \
    src/JointTree.cpp \
    src/Ray.cpp \
    src/Joint.cpp \
    src/SphereCamera.cpp \
    src/Color.cpp \
    src/GLWidget.cpp \
    src/MainWindow.cpp \
    src/OpenGL.cpp \
    src/Plane.cpp \
    src/Point.cpp \
    src/Random.cpp \
    src/Rectangle.cpp \
    src/Vector.cpp \
    src/main.cpp
FORMS += cs4620.ui
RESOURCES += 
