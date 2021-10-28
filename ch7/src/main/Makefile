ifeq ($(RELEASE), 1)
DBG_FLAGS = -O2
else
DBG_FLAGS = -g -O0 -DDEBUG
endif

PWD = $(shell pwd)

#INC_DIRS = -I$(PWD)/../include -I/usr/include/python3.8 -I/usr/include/x86_64-linux-gnu/python3.8
INC_DIRS = -I/usr/include/python3.8 -I/usr/include/x86_64-linux-gnu/python3.8

LIBS = -lpython3.8
#LIBS = -lm -lGL -lglut -lGLU -lgsl
#LIBS = -lm

OBJS = $(SRCS:%.c=%.o)

TARGET = py_interconnect py_noret py_intret py_stringret py_module_complete

#FILE_CONTROL_DIR = $(PWD)/../file

#FILE_CONTROL_OBJS = $(shell find $(FILE_CONTROL_DIR) -name '*.o')
#MATH_TECH_OBJS = $(shell find $(MATH_TECH_DIR) -name '*.o')
#OGL_OBJS = $(shell find $(OGL_DIR) -name '*.o')
#RADAR_PRINCIPLES_OBJS = $(shell find $(RADAR_PRINCIPLES_DIR) -name '*.o')
#RADAR_SIGNAL_PROCESSING_OBJS = $(shell find $(RADAR_SIGNAL_PROCESSING_DIR) -name '*.o')

#LINEAR_ALGEBRA_DIR = $(PWD)/../linear_algebra

#LINEAR_ALGEBRA_OBJS = $(shell find $(LINEAR_ALGEBRA_DIR) -name '*.o')

all:	$(TARGET)

py_interconnect:	py_interconnect.o
	@echo "$<"
	$(CC) -o $@ $< $(LIBS)

py_noret:	py_noret.o
	@echo "$<"
	$(CC) -o $@ $< $(LIBS)

py_intret:	py_intret.o
	@echo "$<"
	$(CC) -o $@ $< $(LIBS)

py_stringret:	py_stringret.o
	@echo "$<"
	$(CC) -o $@ $< $(LIBS)

py_module_complete:	py_module_complete.o
	@echo "$<"
	$(CC) -o $@ $< $(LIBS)
	#$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

.c.o:
	@echo "======================================="
	@echo "========= Compiling Main Prog ========="
	@echo "======================================="
	$(CC) $(DBG_FLAGS) -c $< -o $@ $(INC_DIRS)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
