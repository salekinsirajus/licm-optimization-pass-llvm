# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/susan
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = susan

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = susan.c

# test information
INFILE  = /dev/null
OUTFILE = $(programs)$(EXTRA_SUFFIX).out
ARGS    = /ece566/wolfbench/Benchmarks/susan/input_large.pgm output_large$(EXTRA_SUFFIX).smoothing.pgm -s -d 15
COMPARE = /ece566/wolfbench/Benchmarks/susan/output_large.smoothing.pgm output_large$(EXTRA_SUFFIX).smoothing.pgm /ece566/wolfbench/Benchmarks/susan/output.$(programs) $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
