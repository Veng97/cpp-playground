FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# Install clang format/tidy
RUN apt-get update && apt-get install -y \
    bash-completion \
    git \
    make \
    gdb \
    valgrind \
    clang \
    clang-format \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/*


# Persist bash history via volume
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/bash-history/.bash_history" \
    && echo "$SNIPPET" >> "/root/.profile"

# Avoid git warnings with unsafe directories
RUN git config --global --add safe.directory '*'