#include "lock_guard_from_isr.hpp"

using namespace freertos;

lock_guard_from_isr::lock_guard_from_isr(abstract::semaphore& semaphore):
semaphore(semaphore)
{
    this->owned = this->semaphore.take_from_isr();
}

lock_guard_from_isr::~lock_guard_from_isr(void) {
    if (this->owned){
        this->owned = !this->semaphore.give_from_isr();
    }
}

bool lock_guard_from_isr::owns_lock(void) const {
    return this->owned;
}
