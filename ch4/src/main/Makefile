ifeq ($(RELEASE), 1)
DBG_FLAGS = -O2
else
DBG_FLAGS = -g -O0 -DDEBUG
endif

PWD = $(shell pwd)

INC_DIRS = -I$(PWD)/../include

#LIBS = -lm -lGL -lglut -lGLU -lGLEW -lstdc++
LIBS = -lglut -lGL -lGLU -lgsl -lm
#LIBS = -lm

OBJS = $(SRCS:%.c=%.o)

TARGET = y_sin_x_integral y_sin_x_derivatives ode detail_yprime_exp_x rl_test

#FILE_CONTROL_DIR = $(PWD)/../file

#FILE_CONTROL_OBJS = $(shell find $(FILE_CONTROL_DIR) -name '*.o')
#MATH_TECH_OBJS = $(shell find $(MATH_TECH_DIR) -name '*.o')
#OGL_OBJS = $(shell find $(OGL_DIR) -name '*.o')
#RADAR_PRINCIPLES_OBJS = $(shell find $(RADAR_PRINCIPLES_DIR) -name '*.o')
#RADAR_SIGNAL_PROCESSING_OBJS = $(shell find $(RADAR_SIGNAL_PROCESSING_DIR) -name '*.o')

LINEAR_ALGEBRA_DIR = $(PWD)/../linear_algebra

LINEAR_ALGEBRA_OBJS = $(shell find $(LINEAR_ALGEBRA_DIR) -name '*.o')

all:	$(TARGET)

y_sin_x_integral:	y_sin_x_integral.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

y_sin_x_derivatives:	y_sin_x_derivatives.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

ode:	ode.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

detail_yprime_exp_x:	detail_yprime_exp_x.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

rl_test:	rl_test.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

.c.o:
	@echo "======================================="
	@echo "========= Compiling Main Prog ========="
	@echo "======================================="
	$(CC) $(DBG_FLAGS) -c $< -o $@ $(INC_DIRS)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
