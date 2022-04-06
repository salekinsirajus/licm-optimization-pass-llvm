.PHONY: p3

%.ll:%.bc
	llvm-dis-13 $<
	cat $@

%.o:%.cpp
	clang++-13 -c `llvm-config-13 --cxxflags` -o $@ $<

%.ll:%.c
	clang-13 -c -S -emit-llvm -o $@ $<

%.o:%.bc
	clang++-13 -c -o$@ $<

build/p3: p3.o
	clang++-13 -o$@ $^ `llvm-config-13 --cxxflags --ldflags --libs --system-libs`

clean:
	rm -f p3.o build/p3 *~ main.bc main.ll
