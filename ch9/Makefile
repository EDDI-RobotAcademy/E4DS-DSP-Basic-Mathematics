PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lGL -lglut -lGLU -lm

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

MOUSE_WHEEL_TEST = $(MAIN)/mouse_wheel_test
CAMERA_TEST = $(MAIN)/camera_test
Y_SIN_X_ZOOM = $(MAIN)/y_sin_x_zoom

LINEAR_ALGEBRA = $(SRC)/linear_algebra
SUBDIRS = $(LINEAR_ALGEBRA)

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(MOUSE_WHEEL_TEST) ./build/)
	$(shell cp $(CAMERA_TEST) ./build/)
	$(shell cp $(Y_SIN_X_ZOOM) ./build/)

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
