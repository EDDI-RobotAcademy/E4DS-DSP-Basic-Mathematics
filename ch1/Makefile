PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

#FILE = $(SRC)/file
MAIN = $(SRC)/main
LINEAR_ALGEBRA = $(SRC)/linear_algebra

VECTOR_TEST = $(MAIN)/vector_test

#SUBDIRS = $(FILE)
SUBDIRS = $(LINEAR_ALGEBRA)

all:	main
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(VECTOR_TEST) ./build/)

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

