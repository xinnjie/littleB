cd third_party/folly/folly
mkdir _build && cd _build
cmake -DBUILD_TESTS=off ..
make -j
sudo make install