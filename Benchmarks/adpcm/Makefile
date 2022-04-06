# /* For copyright information, see olden_v1.01/COPYRIGHT */

include ../../Makefile.defs

SRC_DIR=/ece566/wolfbench/Benchmarks/adpcm
INSTALL_DIR=/ece566/install/bin

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

programs = adpcm

.PHONY: all install

all: $(addsuffix $(EXTRA_SUFFIX),$(programs))

install: all

DEFS    = -D__GNUC__ -D_NO_LONGLONG -DPLAIN -DOLDEN

SOURCES = adpcm.c rawdaudio.c

# test information
INFILE  = /ece566/wolfbench/Benchmarks/adpcm/large.pcm
OUTFILE = output_large.adpcm
ARGS    = 
COMPARE = `tar xzf /ece566/wolfbench/Benchmarks/adpcm/output_large.adpcm.tgz; echo output_large.adpcm` $(OUTFILE)

include /ece566/wolfbench/Makefile.benchmark
include ../../Makefile.config
