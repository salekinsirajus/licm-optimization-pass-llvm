# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/qsort
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = qsort

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = qsort_large.c

# test information
INFILE  = /dev/null
OUTFILE = $(programs)$(EXTRA_SUFFIX).out
ARGS    = /ece566/wolfbench/Benchmarks/qsort/input_large.dat
COMPARE = /ece566/wolfbench/Benchmarks/qsort/output.qsort $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
