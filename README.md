# LLVM Optimization Pass: Loop Invariant Code Motion
This is a class project for ECE566: Compiler Optimization and Scheduling at NC State

## Running Locally
Build a docker container using the docker file provided.
```
docker build -t llvmp3:latest .
```

### Run
```
docker run -v $(pwd):/ece566 -it llvmp3:latest /bin/bash
```
