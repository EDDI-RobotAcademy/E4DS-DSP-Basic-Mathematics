PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lGL -lglut -lm -lGLU -lgsl

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

SECOND_ODE = $(MAIN)/second_ode
SECOND_ODE_PLOT = $(MAIN)/second_ode_plot
SOP_PYTHON = $(MAIN)/sode.py

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(SECOND_ODE) ./build/)
	$(shell cp $(SECOND_ODE_PLOT) ./build/)
	$(shell cp $(SOP_PYTHON) ./build/)

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
