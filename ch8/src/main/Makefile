ifeq ($(RELEASE), 1)
DBG_FLAGS = -O2
else
DBG_FLAGS = -g -O0 -DDEBUG
endif

PWD = $(shell pwd)

INC_DIRS = -I$(PWD)/../include

LIBS = -lm -lGL -lglut -lGLU
#LIBS = -lm

OBJS = $(SRCS:%.c=%.o)

TARGET = signal_plot digit_signal_plot ogl_uni_rand_hist ogl_gauss_rand_hist sine_fft256 fft1024_with_6G noisy_wave

#FILE_CONTROL_DIR = $(PWD)/../file

#FILE_CONTROL_OBJS = $(shell find $(FILE_CONTROL_DIR) -name '*.o')
#MATH_TECH_OBJS = $(shell find $(MATH_TECH_DIR) -name '*.o')
#OGL_OBJS = $(shell find $(OGL_DIR) -name '*.o')
#RADAR_PRINCIPLES_OBJS = $(shell find $(RADAR_PRINCIPLES_DIR) -name '*.o')
#RADAR_SIGNAL_PROCESSING_OBJS = $(shell find $(RADAR_SIGNAL_PROCESSING_DIR) -name '*.o')

LINEAR_ALGEBRA_DIR = $(PWD)/../linear_algebra

LINEAR_ALGEBRA_OBJS = $(shell find $(LINEAR_ALGEBRA_DIR) -name '*.o')

all:	$(TARGET)

signal_plot:	signal_plot.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

digit_signal_plot:	digit_signal_plot.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

ogl_uni_rand_hist:	ogl_uni_rand_hist.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

ogl_gauss_rand_hist:	ogl_gauss_rand_hist.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

sine_fft256:	sine_fft256.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

fft1024_with_6G:	fft1024_with_6G.o
	@echo "$<"
	$(CC) -o $@ $< $(LINEAR_ALGEBRA_OBJS) $(LIBS)

noisy_wave:		noisy_wave.o
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
