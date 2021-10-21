PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lglut -lGL -lGLU -lgsl -lm

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

Y_SINX_INTEGRAL = $(MAIN)/y_sin_x_integral
Y_SINX_DERIVATIVE = $(MAIN)/y_sin_x_derivatives
ODE = $(MAIN)/ode
YPRIME_EXPX = $(MAIN)/detail_yprime_exp_x
RL_TEST = $(MAIN)/rl_test

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(Y_SINX_INTEGRAL) ./build/)
	$(shell cp $(Y_SINX_DERIVATIVE) ./build/)
	$(shell cp $(ODE) ./build/)
	$(shell cp $(YPRIME_EXPX) ./build/)
	$(shell cp $(RL_TEST) ./build/)

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
