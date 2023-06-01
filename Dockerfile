# ECE 466/566 container for easy use on Windows, Linux, MacOS

FROM ubuntu:23.04

LABEL maintainer="jtuck@ncsu.edu"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get clean \
  && apt-get install -y --no-install-recommends ssh \
      build-essential \
      gcc \
      g++ \
      gdb \
      cmake \
      rsync \
      tar \
      python3 \
      python-is-python3\
      pip \
      zlib1g-dev \
      bison \
      libbison-dev \
      flex \
   && apt-get clean

RUN apt-get clean \
    && apt-get install -y --no-install-recommends libllvm-13-ocaml-dev libllvm13 llvm-13 llvm-13-dev llvm-13-doc llvm-13-examples llvm-13-runtime clang-13 clang-tools-13 clang-13-doc libclang-common-13-dev libclang-13-dev libclang1-13 clang-format-13 python3-clang-13 clangd-13 libfuzzer-13-dev lldb-13 lld-13 libc++-13-dev libc++abi-13-dev libomp-13-dev libclc-13-dev libunwind-13-dev libfl-dev \
    && apt-get clean

RUN apt-get autoclean
RUN apt-get autoremove

RUN apt-get install -y time \
    git \
    vim \
    && apt-get clean

RUN apt install -y pipx
RUN pipx install lit

ADD . /ece566
ADD . /build
WORKDIR /ece566

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

RUN useradd -m user \
  && yes password | passwd user

COPY . /ece566
COPY .vimrc /root/.vimrc
CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]
