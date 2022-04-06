# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/bisort
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = bisort

.PHONY: all install

all:  $(addsuffix $(EXTRA_SUFFIX),$(programs)) 

install: all 

DEFS    = -D__GNUC__ -D_NO_LONGLONG -DNOTESTP -DONEONLY

SOURCES = swap.c bitonic.c args.c

# test information
INFILE  = /dev/null
OUTFILE = $(programs).out
ARGS    = 2000000 1 5
COMPARE = 

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
