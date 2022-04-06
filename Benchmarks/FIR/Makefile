# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/FIR
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = fir

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = fir.c

# test information
INFILE  = /dev/null
OUTFILE = $(programs)$(EXTRA_SUFFIX).out
ARGS    = /ece566/wolfbench/Benchmarks/FIR/input.pcm
COMPARE = /ece566/wolfbench/Benchmarks/FIR/outputFloat.pcm outputFloat.pcm

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
