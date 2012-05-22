QT -= gui
HEADERS += src/data.h \
    src/input.h \
    src/setup.h \
    src/solve.h \
    src/output.h
SOURCES += src/FlowSim.cpp \
    src/input.cpp \
    src/setup.cpp \
    src/solve.cpp \
    src/output.cpp
OTHER_FILES += run/cfg.txt \
    run/faces.cfg \
    run/cells.cfg \
    run/data.phi \
    run/data.meshY \
    run/data.meshX \
    run/data.cfg
CONFIG += console
