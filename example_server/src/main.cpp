#include <iostream>

#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#include "command_register.h"

#include "services/example_sync_service.h"


using namespace folly;
using namespace std;
using namespace littleB;

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

int main() {
    example_folly();
    return 0;
}