# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/FFT
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = fft

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = main.c fftmisc.c  fourierf.c

# test information
INFILE  = /dev/null
OUTFILE = $(addsuffix $(EXTRA_SUFFIX).txt,output_large)
ARGS    = 8 32768 
COMPARE = /ece566/wolfbench/Benchmarks/FFT/output_large.txt $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
