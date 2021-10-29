PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

#LIBS = -lGL -lglut -lGLU -lgsl -lm
LIBS = -I/usr/include/python3.8 -I/usr/include/x86_64-linux-gnu/python3.8 -lpython3.8

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

PY_INTERCONNECT = $(MAIN)/py_interconnect
PY_NORET = $(MAIN)/py_noret
PY_INTRET = $(MAIN)/py_intret
PY_STRINGRET = $(MAIN)/py_stringret
PY_MODULE_COMPLETE = $(MAIN)/py_module_complete
PYC_LIB = $(MAIN)/pyc_lib.py
PY_MODULE = $(MAIN)/my_module.py

SYMPY_TEST = $(MAIN)/sympy_test.py
SYMPY_LT = $(MAIN)/sympy_laplace_transform.py
SYMPY_HEAVISIDE = $(MAIN)/sympy_heaviside.py
SYMPY_HEAVISIDE_APP = $(MAIN)/sympy_heaviside_app.py
SYMPY_EVAL = $(MAIN)/sympy_eval.py
SYMPY_EVAL_FOREACH = $(MAIN)/sympy_foreach_eval.py

#LINEAR_ALGEBRA = $(SRC)/linear_algebra
#SUBDIRS = $(LINEAR_ALGEBRA)

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(PY_INTERCONNECT) ./build/)
	$(shell cp $(PY_NORET) ./build/)
	$(shell cp $(PY_INTRET) ./build/)
	$(shell cp $(PY_STRINGRET) ./build/)
	$(shell cp $(PY_MODULE_COMPLETE) ./build/)
	$(shell cp $(PYC_LIB) ./build/)
	$(shell cp $(PY_MODULE) ./build/)
	$(shell cp $(SYMPY_TEST) ./build/)
	$(shell cp $(SYMPY_LT) ./build/)
	$(shell cp $(SYMPY_HEAVISIDE) ./build/)
	$(shell cp $(SYMPY_HEAVISIDE_APP) ./build/)
	$(shell cp $(SYMPY_EVAL) ./build/)
	$(shell cp $(SYMPY_EVAL_FOREACH) ./build/)

#main:	_all
main:
	$(MAKE) -C $(MAIN)

#_all:
	#for dir in $(SUBDIRS); do \
	#	$(MAKE) -C $$dir; \
	#done

clean:  mclear
	$(shell rm -rf build)

mclear:	_clean
	$(MAKE) -C $(MAIN) clean

_clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
