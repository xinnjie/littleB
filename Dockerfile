FROM ubuntu:18.04
COPY minigame_server /apps/minigame_server
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
    libprotobuf-dev
EXPOSE 10002
CMD /apps/minigame_server/bin/minigame_server
