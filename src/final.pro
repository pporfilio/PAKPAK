QT += core \
    gui \
    opengl
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
    lib/targa.h \
    main/glwidget.h \
    math/CS123Matrix.h \
    math/CS123Algebra.h \
    math/CS123Vector.h \
    lib/utils.h \
    lib/CS123Common.h \
    support/OrbitingCamera.h \
    support/GameCamera.h \
    support/camera.h
SOURCES += main/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    math/CS123Matrix.cpp \
    math/CS123Matrix.inl \
    math/CS123Vector.inl \
    lib/utils.cpp \
    support/OrbitingCamera.cpp \
    support/GameCamera.cpp
FORMS += mainwindow.ui \
    support/mainwindow.ui
OTHER_FILES += shaders/fractal.frag \
    shaders/fractal.vert \
    shaders/mandelbox.vert \
    shaders/mandelbox.frag
RESOURCES += 
