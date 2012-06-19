QT -= gui
HEADERS += src/data.h \
    src/input.h \
    src/setup.h \
    src/output.h
SOURCES += src/GridCreate.cpp \
    src/data.cpp \
    src/input.cpp \
    src/setup.cpp \
    src/output.cpp
CONFIG += console
OTHER_FILES += \
    run/cfg.txt \
    run/faces.cfg \
    run/cells.cfg
