# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/em3d
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = em3d

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs)) 

install: all 

DEFS    = -D__GNUC__ -D_NO_LONGLONG -DPLAIN -DOLDEN

SOURCES = em3d.c main.c make_graph.c util.c args.c

# test information
INFILE  = /dev/null
OUTFILE = $(programs)$(EXTRA_SUFFIX).out
ARGS    = 160000 15 88
COMPARE = /ece566/wolfbench/Benchmarks/em3d/output.em3d $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
