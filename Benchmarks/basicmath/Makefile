# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/basicmath
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = basicmath

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = basicmath_large.c   rad2deg.c  cubic.c   isqrt.c

# test information
INFILE  = /dev/null
OUTFILE = output_large$(EXTRA_SUFFIX).txt
ARGS    = 
COMPARE = $(OUTFILE) /ece566/wolfbench/Benchmarks/basicmath/output_large.txt

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
