TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Cube.cpp \
    Initialize.cpp \
    Instance.cpp \
    main.cpp \
    Object3D.cpp

HEADERS += \
    Cube.h \
    GraphicsEngine3D.h \
    Initialize.h \
    Instance.h \
    Polygon3D.h \
    Object3D.h

DISTFILES += \
    Pyramid.obj
