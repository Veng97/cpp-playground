FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# Common dependencies
RUN apt-get update && apt-get install -y \
    bash-completion \
    git \
    cmake \
    clang \
    clang-format \
    clang-tidy \
    clangd \
    gdb \
    valgrind \
    wget \
    lsb-release \
    software-properties-common \
    gnupg \
    && rm -rf /var/lib/apt/lists/*

# Install GoogleTest
RUN apt-get update && apt-get install -y \
    googletest \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src/googletest \
    && cmake . \
    && make \
    && make install

# Setup user with sudo privileges
RUN apt-get update && apt-get install -y \
    sudo \
    && rm -rf /var/lib/apt/lists/* \
    && echo "ubuntu ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers \
    && usermod -aG sudo,adm,dialout,audio,video ubuntu

USER ubuntu