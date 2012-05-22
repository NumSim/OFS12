QT -= gui
HEADERS += \
    src/data.h \
    src/input.h \
    src/setup.h \
    src/solve.h \
    src/output.h
SOURCES += \
    src/main.cpp \
    src/data.cpp \
    src/input.cpp \
    src/setup.cpp \
    src/solve.cpp \
    src/output.cpp
OTHER_FILES += \
    run/cfg.txt \
    run/scalar.dat \
    run/phyGrid.meshY \
    run/phyGrid.meshX
CONFIG += console
