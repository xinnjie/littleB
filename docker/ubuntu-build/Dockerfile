# run at littleB
ARG PARALLEL
ARG INSTALL_DIR=/install
FROM xinnjie/littleb-ubuntu-build-env as builder
COPY . /src/littleB
WORKDIR /src/littleB
RUN  mkdir _build && cd _build && cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR .. && make install -j$PARALEL

