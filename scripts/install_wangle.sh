set -x
cd third_party/wangle/wangle
mkdir _build && cd _build
cmake -DBUILD_TESTS=off ..
make -j$PARALLEL_ENV
make install