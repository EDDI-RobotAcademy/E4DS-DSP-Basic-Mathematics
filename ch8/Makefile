PWD = $(shell pwd)
SRC = $(PWD)/src
INC = $(PWD)/include

MAIN = $(SRC)/main

LIBS = -lGL -lglut -lGLU -lgsl -lm

SRCS = $(wildcard *.c)
TARGET = $(SRCS:%.c=%)

SIGNAL_PLOT = $(MAIN)/signal_plot
DIGIT_SIGNAL_PLOT = $(MAIN)/digit_signal_plot
UNI_RAND_HIST = $(MAIN)/ogl_uni_rand_hist
GAUSS_RAND_HIST = $(MAIN)/ogl_gauss_rand_hist
SINE_FFT256 = $(MAIN)/sine_fft256
FFT1024_6G = $(MAIN)/fft1024_with_6G
NOISY_WAVE = $(MAIN)/noisy_wave

LINEAR_ALGEBRA = $(SRC)/linear_algebra
SUBDIRS = $(LINEAR_ALGEBRA)

all: main
	@echo $(TARGET)
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(SIGNAL_PLOT) ./build/)
	$(shell cp $(DIGIT_SIGNAL_PLOT) ./build/)
	$(shell cp $(UNI_RAND_HIST) ./build/)
	$(shell cp $(GAUSS_RAND_HIST) ./build/)
	$(shell cp $(SINE_FFT256) ./build/)
	$(shell cp $(FFT1024_6G) ./build/)
	$(shell cp $(NOISY_WAVE) ./build/)

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
