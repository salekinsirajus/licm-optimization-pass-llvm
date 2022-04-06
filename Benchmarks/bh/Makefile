# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/bh
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs=bh

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all

DEFS    = -D__GNUC__ -D_NO_LONGLONG

SOURCES = newbh.c util.c args.c

# test information
INFILE  = /dev/null
OUTFILE = $(addsuffix $(EXTRA_SUFFIX).out,$(programs))
ARGS    = 11000 1
COMPARE = /ece566/wolfbench/Benchmarks/bh/output.bh $(addsuffix $(EXTRA_SUFFIX).out,$(programs))

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
