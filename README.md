# LLVM Optimization Pass: Loop Invariant Code Motion
This is a class project for ECE566: Compiler Optimization and Scheduling at NC State

## Running Locally
Build a docker container using the docker file provided.
```
docker build -t llvmp3:latest .
```

### Build Executable
Get into the docker container
```
docker run -v $(pwd):/ece566 -it llvmp3:latest /bin/bash
```
Create a build directory
```
mkdir build
```
Run cmake
```
cmake ..
```
Now run make
```
make
```

## Building Test
Create a test directory in the root folder and `cd` into it
```
mkdir test
cd test
```
Configure the benchmark tool to find the executable that we built earlier
```
/ece566/wolfbench/wolfbench/configure --enable-customtool=/ece566/build/p3
```

## Running Test
while you are in the `test` directory, run the following command to compile
all the benchmark.
```
make all test
```
To ensure that our optimization pass did not break things, run the following:
```
make test compare
```