PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lGL -lglut -lm -lglu

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

GL_WINDOW = $(MAIN)/gl_window
XY_AXIS = $(MAIN)/xy_axis
Y_X_PLOT = $(MAIN)/y_x_plot
XYZ_AXIS = $(MAIN)/xyz_axis
DRAW_CONE = $(MAIN)/draw_cone
VECTOR_OPERATION_PLOT = $(MAIN)/vector_operation_plot

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(GL_WINDOW) ./build/)
	$(shell cp $(XY_AXIS) ./build/)
	$(shell cp $(Y_X_PLOT) ./build/)
	$(shell cp $(XYZ_AXIS) ./build/)
	$(shell cp $(DRAW_CONE) ./build/)
	$(shell cp $(VECTOR_OPERATION_PLOT) ./build/)

main:	_all
	$(MAKE) -C $(MAIN)

_all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:  mclear
	$(shell rm -rf build)

mclear:	_clean
	$(MAKE) -C $(MAIN) clean

_clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
