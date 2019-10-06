FROM ubuntu:18.04
# install tool-chain
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake
RUN apt-get update && apt-get install -y \
    libboost-all-dev
RUN apt-get update && apt-get install -y \
    libevent-dev \
    libdouble-conversion-dev \
    libgoogle-glog-dev \
    libgflags-dev \
    libiberty-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    zlib1g-dev \
    binutils-dev \
    libjemalloc-dev \
    libssl-dev
RUN apt-get update && apt-get install -y \
    libunwind8-dev \
    libelf-dev \
    libdwarf-dev \
    libsodium-dev \
    libprotobuf-dev \
    protobuf-compiler