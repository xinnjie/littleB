cd third_party/folly
mkdir _build && cd _build
cmake -DBUILD_TESTS=off ..
make -j2
sudo make install