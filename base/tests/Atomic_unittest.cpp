//
// Created by mark on 11/21/16.
//

#include <blackpanther/base/Atomic.h>

#include <iostream>

#include <assert.h>

using namespace blackpanther;
AtomicInt32 g_AtomicInt32;

int main(){
    assert(g_AtomicInt32.get() == 0);
    assert(g_AtomicInt32.getAndAdd(5) == 0);
    assert(g_AtomicInt32.get() == 5);
    assert(g_AtomicInt32.addAndGet(2) == 7);
    assert(g_AtomicInt32.incrementAndGet() == 8);

    g_AtomicInt32.increment();
    assert(g_AtomicInt32.get() == 9);

    g_AtomicInt32.decrement();
    assert(g_AtomicInt32.get() == 8);

    assert(g_AtomicInt32.decrementAndGet() == 7);

    g_AtomicInt32.getAndSet(1000);
    std::cout << g_AtomicInt32.get() << std::endl;
    return 0;
}