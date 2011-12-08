QT += core gui opengl

TARGET = final
TEMPLATE = app

INCLUDEPATH += main \
    lib \
    math \
    support
DEPENDPATH += main \
    lib \
    math \
    support

HEADERS += main/glwidget.h \
    lib/targa.h \
    lib/glm.h \
    math/vector.h \
    support/resourceloader.h \
    support/mainwindow.h \
    support/camera.h \
    lib/targa.h \
    main/glwidget.h \
    math/CS123Matrix.h \
    math/CS123Algebra.h \
    math/CS123Vector.h \
    lib/utils.h \
    lib/CS123Common.h \
    support/testCamera.h \
    support/OrbitingCamera.h

SOURCES += \
    main/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    math/CS123Matrix.cpp \
    math/CS123Matrix.inl \
    math/CS123Vector.inl \
    lib/utils.cpp \
    support/OrbitingCamera.cpp

FORMS += mainwindow.ui \
    support/mainwindow.ui

OTHER_FILES += \
    shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag \
    shaders/fractal.frag \
    shaders/fractal.vert \
    shaders/test.vert

RESOURCES +=



















