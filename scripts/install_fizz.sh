cd third_party/fizz/fizz
mkdir _build && cd _build
cmake -DBUILD_TESTS=off -DCMAKE_C_COMPILER=/usr/bin/clang  -DCMAKE_CXX_COMPILER=/usr/bin/g++..
make -j2
sudo make install