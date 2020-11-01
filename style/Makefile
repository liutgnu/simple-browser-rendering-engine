SOURCES = style_node_parser.cc \
			style_node.cc

MAIN_SOURCE = main.cc
UTILS_SOURCE = ../utils/font_manager.cc
LIBRARY_OUT = style.o
INTERGRATE_TEST_OUT = styleprog.out

EXTRA_CFLAGS = -I/usr/include/SDL2 \
				-I/usr/include/freetype2 \
				-I/usr/include/libpng16

EXTRA_LDFLAGS = -lSDL2 \
				-lfreetype \
				-lSDL2_image

LIBRARY_EXTRAS = ../css/css.o ../html/html.o 

all: $(LIBRARY_OUT)
include ../Makefile.common