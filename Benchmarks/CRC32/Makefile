# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/CRC32
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = crc32

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all 

DEFS    = 

SOURCES = crc_32.c

# test information
INFILE  = /dev/null
OUTFILE = output_large$(EXTRA_SUFFIX).out
ARGS    = /ece566/wolfbench/Benchmarks/adpcm/large.pcm
COMPARE= /ece566/wolfbench/Benchmarks/CRC32/output_large.txt output_large$(EXTRA_SUFFIX).out

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
