SOURCES = sdl_drawer.cc \
			sdl_drawer_interface.cc

MAIN_SOURCE = main.cc
UTILS_SOURCE = ../utils/font_manager.cc
LIBRARY_OUT = drawer.o
INTEGRATE_TEST_OUT = drawerprog.out

EXTRA_CFLAGS = -I/usr/include/SDL2 \
				-I/usr/include/freetype2 \
				-I/usr/include/libpng16

EXTRA_LDFLAGS = -lSDL2 \
				-lfreetype \
				-lSDL2_image

LIBRARY_EXTRAS = ../layout/layout.o \
				../style/style.o \
				../html/html.o \
				../css/css.o

all: $(LIBRARY_OUT)
include ../Makefile.common