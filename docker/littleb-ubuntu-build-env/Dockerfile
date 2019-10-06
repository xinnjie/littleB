FROM xinnjie/littleb-ubuntu-build-base
ARG PARALLEL
COPY third_party /third_party
COPY scripts /scripts
WORKDIR /
ENV PARALLEL_ENV=$PARALLEL
RUN bash scripts/install_folly.sh
RUN bash scripts/install_fizz.sh
RUN bash scripts/install_wangle.sh
# clean build files
RUN rm -rf /scripts && rm -rf /third_party
