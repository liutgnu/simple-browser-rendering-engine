SOURCES = layout_node.cc

MAIN_SOURCE = main.cc
UTILS_SOURCE = ../utils/font_manager.cc
LIBRARY_OUT = layout.o
INTEGRATE_TEST_OUT = layoutprog.out

EXTRA_CFLAGS = -I/usr/include/SDL2 \
				-I/usr/include/freetype2 \
				-I/usr/include/libpng16

EXTRA_LDFLAGS = -lSDL2 \
				-lfreetype \
				-lSDL2_image

LIBRARY_EXTRAS = ../style/style.o \
				../html/html.o \
				../css/css.o

all: $(LIBRARY_OUT)
include ../Makefile.common