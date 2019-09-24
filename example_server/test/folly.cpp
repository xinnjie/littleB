#include <iostream>

#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
using namespace folly;
using namespace std;

void foo(int x) {
    // do something with x
    cout << "foo(" << x << ")" << endl;
}

void example_folly() {
    // ...
    folly::ThreadedExecutor executor;
    cout << "making Promise" << endl;
    Promise<int> p;
    Future<int> f = p.getSemiFuture().via(&executor);
    auto f2 = move(f).thenValue(foo);
    cout << "Future chain made" << endl;

    // ... now perhaps in another event callback

    cout << "fulfilling Promise" << endl;
    p.setValue(42);
    move(f2).get();
    cout << "Promise fulfilled" << endl;
}

class Base {
public:
    int i;
    Base(int i) : i(i) {}
};
class Derive : public Base {
public:
    int j;
    Derive(int i, int j) : Base(i), j(j) {}
};


TEST_CASE( "folly inherit test", "[derive & base]" ) {
    Promise<Derive> derive_promise;
    Future<Base> base_future = derive_promise.getFuture();
    REQUIRE_FALSE(base_future.isReady());

    derive_promise.setValue(Derive(1, 2));
    REQUIRE(base_future.isReady());
    REQUIRE(base_future.value().i == 1);

}
