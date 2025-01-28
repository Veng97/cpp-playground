FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# Common dependencies
RUN apt-get update && apt-get install -y \
    bash-completion \
    git \
    cmake \
    gdb \
    valgrind \
    wget \
    lsb-release \
    software-properties-common \
    gnupg \
    && rm -rf /var/lib/apt/lists/*

# Dependencies for building/scanning/formatting (clang, clang-format, clang-tidy, clangd)
ARG LLVM_VERSION=18
RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh ${LLVM_VERSION} \
    && rm llvm.sh \
    && ln -sf /usr/bin/clang++-${LLVM_VERSION} /usr/bin/clang++ \
    && ln -sf /usr/bin/clang-${LLVM_VERSION} /usr/bin/clang \
    && ln -sf /usr/bin/clang-tidy-${LLVM_VERSION} /usr/bin/clang-tidy \
    && ln -sf /usr/bin/clang-format-${LLVM_VERSION} /usr/bin/clang-format \
    && ln -sf /usr/bin/clangd-${LLVM_VERSION} /usr/bin/clangd \
    && rm -rf /var/lib/apt/lists/*

# Install GoogleTest
RUN apt-get update && apt-get install -y \
    googletest \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src/googletest \
    && cmake . \
    && make \
    && make install

# Avoid git warnings with unsafe directories
RUN git config --global --add safe.directory '*'