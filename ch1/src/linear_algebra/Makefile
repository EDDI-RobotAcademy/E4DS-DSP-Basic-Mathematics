ifeq ($(RELEASE), 1)
DBG_FLAGS = -O2
else
DBG_FLAGS = -g -O0 -DDEBUG
endif

PWD = $(shell pwd)

INC_DIRS = -I$(PWD)/../include

#SRCS = $(patsubst %.o,%,$(wildcard *.c))
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)

all:    $(OBJS)
	@echo "SRCS = $(SRCS)"

.c.o:
	@echo "=============================================="
	@echo "========== Compiling Linear Algebra =========="
	@echo "=============================================="
	$(CC) $(DBG_FLAGS) -c $< -o $@ $(INC_DIRS)

clean:
	rm -rf *.o

