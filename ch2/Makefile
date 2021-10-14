PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lGL -lglut -lm

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

MAT3_TEST = $(MAIN)/mat3_test

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(MAT3_TEST) ./build/)

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
