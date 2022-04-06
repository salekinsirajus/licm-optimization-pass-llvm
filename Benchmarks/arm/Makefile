# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/arm
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = arm

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs)) 

install: all

DEFS    = -D__GNUC__ -D_NO_LONGLONG -DPLAIN -DOLDEN

SOURCES = arm.c

# test information
INFILE  = /ece566/wolfbench/Benchmarks/arm/i.4
OUTFILE = $(addsuffix $(EXTRA_SUFFIX),$(programs)).output.stderr
ARGS    = 
COMPARE = /ece566/wolfbench/Benchmarks/arm/arm.output.stderr $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
